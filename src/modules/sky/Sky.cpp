#include "Sky.hpp"

namespace mod::sky {

    Sky::Sky(const std::string& propsFilePath)
        : m_CurrentState(0)
        , m_NextSwitch(0.0f)
        , m_WaitReset(false)
        , m_LastAngle(0.0f)
    {
        auto pProps = vd::loader::PropertiesLoader::Load(propsFilePath);

        for (int i = 0; ; ++i) {
            m_RotationSpeed = pProps->Get<float>("RotationSpeed");

            const std::string prefix = "State." + std::to_string(i);
            try {
                State s;

                s.Name = pProps->Get<std::string>(prefix + ".Name");

                auto type = pProps->Get<std::string>(prefix + ".Type");
                if (type == "Color") {
                    s.UseColor = true;

                    s.Color = pProps->Get<glm::vec3>(prefix + ".Color");
                    s.ColorFactor = pProps->Get<glm::vec3>(prefix + ".ColorFactor");
                } else if (type == "Texture") {
                    s.UseColor = false;

                    const std::string cubePrefix = prefix + ".Cube";

                    s.CubePaths[FaceType::eRight] = pProps->Get<std::string>(cubePrefix + ".Right");
                    s.CubePaths[FaceType::eLeft] = pProps->Get<std::string>(cubePrefix + ".Left");
                    s.CubePaths[FaceType::eTop] = pProps->Get<std::string>(cubePrefix + ".Top");
                    s.CubePaths[FaceType::eBottom] = pProps->Get<std::string>(cubePrefix + ".Bottom");
                    s.CubePaths[FaceType::eBack] = pProps->Get<std::string>(cubePrefix + ".Back");
                    s.CubePaths[FaceType::eFront] = pProps->Get<std::string>(cubePrefix + ".Front");
                }

                vd::time::Time startAt(uint8_t(pProps->Get<int>(prefix + ".StartAt")), 0);
                s.StartAtAngle = AngleTransform(startAt.ToAngle(), startAt.AM());

                vd::time::Time midAt(uint8_t(pProps->Get<int>(prefix + ".MidAt")), 0);
                s.MidAtAngle = AngleTransform(midAt.ToAngle(), midAt.AM());

                vd::time::Time endAt(uint8_t(pProps->Get<int>(prefix + ".EndAt")), 0);
                s.EndAtAngle = AngleTransform(endAt.ToAngle(), endAt.AM());

                const std::string fogPrefix = prefix + ".Fog";

                s.LowerLimit = pProps->Get<float>(fogPrefix + ".Lower");
                s.UpperLimit = pProps->Get<float>(fogPrefix + ".Upper");
                s.FogColor = pProps->Get<glm::vec3>(fogPrefix + ".Color");

                m_States.emplace_back(s);
            } catch (std::invalid_argument& e) {
                break;
            }
        }
    }

    void Sky::Link() {
        m_pTimeManager = vd::ObjectOfType<vd::time::TimeManager>::Find();
        m_pFogManager = vd::ObjectOfType<vd::fog::FogManager>::Find();
    }

    void Sky::Setup() {
        // Build mesh
        vd::model::Mesh3DPtr pMesh = std::make_shared<vd::model::Mesh3D>();

        for (int i = 0; i < 24; i += 3) {
            pMesh->Vertices().emplace_back(
                    glm::vec3(kSkyboxVertices[i], kSkyboxVertices[i + 1], kSkyboxVertices[i + 2]),
                    glm::vec3(0.0f, 1.0f, 0.0f),
                    glm::vec2(kSkyboxVertices[i], kSkyboxVertices[i + 1])
            );
        }

        pMesh->Indices() = kSkyboxIndices;

        this->PushMesh({ pMesh }, 1000.0f);

        // Fetch textures
        for (auto& state : m_States) {
            if (!state.UseColor) {
                state.Texture = vd::service::TextureService::CubeMapFromFiles(state.CubePaths);
            } else {
                state.Texture = nullptr;
            }
        }

        // Compute current state
        float angle = AngleTransform(m_pTimeManager->CurrentAngle(), m_pTimeManager->AM());
        for (int i = 0; i < m_States.size(); ++i) {
            auto& s = m_States[i];

            if (s.StartAtAngle < s.EndAtAngle) {
                if (angle >= s.StartAtAngle && angle < s.EndAtAngle) {
                    m_WaitReset = false;
                } else {
                    continue;
                }
            } else {
                if (angle < s.EndAtAngle) {
                    m_WaitReset = false;
                } else if (angle > s.StartAtAngle) {
                    m_WaitReset = true;
                } else {
                    continue;
                }
            }

            m_CurrentState = i;
            m_NextSwitch = s.EndAtAngle;
            m_LastAngle = angle;
            UpdateFog(m_States[m_CurrentState]);
            break;
        }
    }

    void Sky::Update() {
        float angle = AngleTransform(m_pTimeManager->CurrentAngle(), m_pTimeManager->AM());
        if (m_WaitReset && angle < 180.0f && m_LastAngle >= 180.0f) {
            m_WaitReset = false;
        }

        if (!m_WaitReset && angle > m_NextSwitch) {
            m_CurrentState = (m_CurrentState + 1) % m_States.size();
            
            if (m_States[m_CurrentState].StartAtAngle > m_States[m_CurrentState].EndAtAngle) {
                m_WaitReset = true;
            }
            m_NextSwitch = m_States[m_CurrentState].EndAtAngle;
        }

        auto& state = m_States[m_CurrentState];
        auto& stateAfter = m_States[(m_CurrentState == m_States.size() - 1) ? 0 : (m_CurrentState + 1)];

        if (m_WaitReset) {
            // mid after 0 and angle before reset, so angle is less than mid
            bool midAfter0AngleBefore = (state.MidAtAngle < 180.0f && angle > 180.0f);
            // mid before 0 and angle before mid, so angle is less than mid
            bool midBefore0 = (state.MidAtAngle > 180.0f && angle < state.MidAtAngle);

            // basically check if angle is less than mid angle
            if (midAfter0AngleBefore || midBefore0) {
                m_Details.Mixable = false;
                m_Details.Percentage = 100.0f;
                SetDetailsFactor(m_Details.First, state);
                ClearDetailsFactor(m_Details.Second);
                m_Details.FogColor = UpdateFog(state);
            } else {
                // if wait reset is true, it means end angle has to be after 0 and current angle before 0, so we need to
                // shift values with 2 dials (180 degrees) to perform proper computations
                // if current angle is before 0, mid angle has to be before 0 and before current angle

                // if end angle + 180.0f will pass 360.0f it will not be a problem mathematically speaking because
                // computations will return the same value (in order to work this formula we need that left < value < right)
                float shLeft = vd::math::ReduceAngle(state.MidAtAngle + 180.0f);
                float shAngle = vd::math::ReduceAngle(angle + 180.0f);
                float shRight = state.EndAtAngle + 180.0f;

                m_Details.Percentage = glm::abs(shAngle - shLeft) / (shRight - shLeft);
                m_Details.Mixable = true;
                SetDetailsFactor(m_Details.First, state);
                SetDetailsFactor(m_Details.Second, stateAfter);
                m_Details.FogColor = UpdateFogMixable(state, stateAfter, m_Details.Percentage);
            }
        } else {
            if (angle < state.MidAtAngle) {
                m_Details.Mixable = false;
                m_Details.Percentage = 100.0f;
                SetDetailsFactor(m_Details.First, state);
                ClearDetailsFactor(m_Details.Second);
                m_Details.FogColor = UpdateFog(state);
            } else {
                m_Details.Percentage = glm::abs(angle - state.MidAtAngle) / (state.EndAtAngle - state.MidAtAngle);
                m_Details.Mixable = true;
                SetDetailsFactor(m_Details.First, state);
                SetDetailsFactor(m_Details.Second, stateAfter);
                m_Details.FogColor = UpdateFogMixable(state, stateAfter, m_Details.Percentage);
            }
        }

        m_LastAngle = angle;

        m_Details.Rotation = vd::math::ReduceAngle(m_RotationSpeed * angle);
    }

    const Sky::RenderDetails& Sky::Details() const {
        return m_Details;
    }

    void Sky::SetDetailsFactor(RenderDetails::Factor& factor, const Sky::State& source) {
        factor.UseColor = source.UseColor;

        if (source.UseColor) {
            factor.Color = source.Color;
            factor.Factor = source.ColorFactor;
            factor.Texture = nullptr;
        } else {
            factor.Color = glm::vec3(0.0f);
            factor.Factor = glm::vec3(0.0f);
            factor.Texture = source.Texture;
        }

        factor.FogLimits = glm::vec2(source.LowerLimit, source.UpperLimit);
    }

    void Sky::ClearDetailsFactor(Sky::RenderDetails::Factor& factor) {
        factor.UseColor = true;
        factor.Color = glm::vec3(0.0f);
        factor.Factor = glm::vec3(0.0f);
        factor.Texture = nullptr;
        factor.FogLimits = glm::vec2(0.0f, 0.0f);
    }

    float Sky::AngleTransform(float angle, bool shift) {
        return (angle * 0.5f) + (shift ? 0.0f : 180.0f);
    }

    glm::vec3 Sky::UpdateFog(const Sky::State& state) {
        m_pFogManager->FogColor(state.FogColor);
        return state.FogColor;
    }

    glm::vec3 Sky::UpdateFogMixable(const Sky::State& left, const Sky::State& right, float factor) {
        glm::vec3 fogColor = glm::mix(left.FogColor, right.FogColor, factor);
        m_pFogManager->FogColor(fogColor);
        return fogColor;
    }

}
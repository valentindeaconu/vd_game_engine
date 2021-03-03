#include "Sky.hpp"

namespace mod::sky {

    Sky::Sky(const std::string& propsFilePath)
        : m_CurrentState(0)
        , m_CurrentColor(0.0f)
        , m_CurrentFactor(0.0f)
    {
        auto pProps = vd::loader::PropertiesLoader::Load(propsFilePath);

        for (int i = 0; ; ++i) {
            const std::string prefix = "State." + std::to_string(i);
            try {
                State s;

                s.Name = pProps->Get<std::string>(prefix + ".Name");
                s.Color = pProps->Get<glm::vec3>(prefix + ".Color");
                s.ColorFactor = pProps->Get<glm::vec3>(prefix + ".ColorFactor");

                vd::time::Time startAt(uint8_t(pProps->Get<int>(prefix + ".StartAt")), 0);
                s.StartAtAngle = (startAt.ToAngle() * 0.5f) + (startAt.AM() ? 0.0f : 180.0f);

                vd::time::Time endAt(uint8_t(pProps->Get<int>(prefix + ".EndAt")), 0);
                s.EndAtAngle = (endAt.ToAngle() * 0.5f) + (endAt.AM() ? 0.0f : 180.0f);

                s.Mixable.Enable = pProps->Get<int>(prefix + ".Mixable");

                if (s.Mixable.Enable) {
                    const std::string mixablePrefix = prefix + ".Mixable";

                    vd::time::Time mStartAt(uint8_t(pProps->Get<int>(mixablePrefix + ".StartAt")), 0);
                    s.Mixable.StartAtAngle = (mStartAt.ToAngle() * 0.5f) + (mStartAt.AM() ? 0.0f : 180.0f);

                    vd::time::Time mMidAt(uint8_t(pProps->Get<int>(mixablePrefix + ".MidAt")), 0);
                    s.Mixable.MidAtAngle = (mMidAt.ToAngle() * 0.5f) + (mMidAt.AM() ? 0.0f : 180.0f);

                    vd::time::Time mEndAt(uint8_t(pProps->Get<int>(mixablePrefix + ".EndAt")), 0);
                    s.Mixable.EndAtAngle = (mEndAt.ToAngle() * 0.5f) + + (mEndAt.AM() ? 0.0f : 180.0f);
                }

                m_States.emplace_back(s);
            } catch (std::invalid_argument& e) {
                break;
            }
        }
    }

    void Sky::Link() {
        m_pTimeManager = vd::ObjectOfType<vd::time::TimeManager>::Find();
    }

    void Sky::Setup() {
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

        for (int i = 0; i < m_States.size(); ++i) {
            auto& s = m_States[i];
            vd::Logger::log(std::to_string(i) + " " + s.Name + " " + std::to_string(s.StartAtAngle) + " " + std::to_string(s.EndAtAngle));
        }

        // Compute current state
        float angle = (m_pTimeManager->CurrentAngle() * 0.5f) + (m_pTimeManager->AM() ? 0.0f : 180.0f);
        for (int i = 0; i < m_States.size(); ++i) {
            auto& s = m_States[i];

            if (angle >= std::min(s.StartAtAngle, s.EndAtAngle) && angle < std::max(s.StartAtAngle, s.EndAtAngle)) {
                m_CurrentState = i;

                if (m_States[m_CurrentState].StartAtAngle > m_States[m_CurrentState].EndAtAngle) {
                    m_WaitReset = true;
                }

                m_NextSwitch = m_States[m_CurrentState].EndAtAngle;
                m_LastAngle = angle;
                break;
            }
        }
    }

    void Sky::Update() {
        float angle = (m_pTimeManager->CurrentAngle() * 0.5f) + (m_pTimeManager->AM() ? 0.0f : 180.0f);
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
        if (state.Mixable.Enable) {
            size_t stateBeforeId = (m_CurrentState == 0) ? (m_States.size() - 1) : (m_CurrentState - 1);
            size_t stateAfterId = (m_CurrentState == m_States.size() - 1) ? 0 : (m_CurrentState + 1);

            auto& stateBefore = m_States[stateBeforeId];
            auto& stateAfter = m_States[stateAfterId];

            float left, right;
            if (angle < state.Mixable.MidAtAngle) {
                left = state.Mixable.StartAtAngle;
                right = state.Mixable.MidAtAngle;
            } else {
                left = state.Mixable.MidAtAngle;
                right = state.Mixable.EndAtAngle;
            }

            float percentage = (angle - left) / (right - left);

            if (angle < state.Mixable.MidAtAngle) {
                m_CurrentColor = glm::mix(stateBefore.Color, state.Color, percentage);
                m_CurrentFactor = glm::mix(stateBefore.ColorFactor, state.ColorFactor, percentage);
            } else {
                m_CurrentColor = glm::mix(state.Color, stateAfter.Color, percentage);
                m_CurrentFactor = glm::mix(state.ColorFactor, stateAfter.ColorFactor, percentage);
            }
        } else {
            m_CurrentColor = state.Color;
            m_CurrentFactor = state.ColorFactor;
        }

        m_LastAngle = angle;
    }

    const glm::vec3& Sky::Color() const {
        return m_CurrentColor;
    }

    const glm::vec3& Sky::ColorFactor() const {
        return m_CurrentFactor;
    }

    size_t Sky::StateAtAngle(float angle) {


        return 0;
    }

}
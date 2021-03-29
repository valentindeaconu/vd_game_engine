//
// Created by Vali on 2/17/2021.
//

#ifndef VDGE_SKY_HPP
#define VDGE_SKY_HPP

#include <engine/object/Entity3D.hpp>

#include <engine/loader/PropertiesLoader.hpp>
#include <engine/service/TextureService.hpp>
#include <engine/time/Time.hpp>

#include <engine/injector/Injectable.hpp>
#include <engine/time/TimeManager.hpp>
#include <engine/fog/FogManager.hpp>

#include <engine/math/Utils.hpp>

#include <memory>

namespace mod::sky {
    class Sky : public vd::object::Entity3D, public vd::injector::Injectable {
    public:
        struct RenderDetails {
            struct Factor {
                bool                        UseColor;
                glm::vec3                   Color;
                glm::vec3                   Factor;
                vd::gl::TextureCubeMapPtr   Texture;
                glm::vec2                   FogLimits;
            };
            Factor      First;
            Factor      Second;
            glm::vec3   FogColor;
            bool        Mixable;
            float       Percentage;
            float       Rotation;
        };

        explicit Sky(const std::string& propsFilePath);

        void Link() override;
        void Setup() override;
        void Update() override;

        [[nodiscard]] const RenderDetails& Details() const;
    private:
        typedef vd::service::TextureService::CubeMapFacesPathVec    PathVec;
        typedef vd::service::TextureService::CubeMapFaceType        FaceType;
        struct State {
            std::string                 Name;
            bool                        UseColor;
            glm::vec3                   Color;
            glm::vec3                   ColorFactor;
            PathVec                     CubePaths;
            vd::gl::TextureCubeMapPtr   Texture;
            float                       StartAtAngle;
            float                       MidAtAngle;
            float                       EndAtAngle;
            glm::vec3                   FogColor;
            float                       LowerLimit;
            float                       UpperLimit;
        };

        static void SetDetailsFactor(RenderDetails::Factor& factor, const State& source);
        static void ClearDetailsFactor(RenderDetails::Factor& factor);
        static float AngleTransform(float angle, bool shift);

        glm::vec3 UpdateFog(const Sky::State& state);
        glm::vec3 UpdateFogMixable(const Sky::State& left, const Sky::State& right, float factor);

        size_t                  m_CurrentState;
        float                   m_NextSwitch;
        float                   m_LastAngle;
        bool                    m_WaitReset;
        std::vector<State>      m_States;
        RenderDetails           m_Details;

        float                   m_RotationSpeed;

        const std::vector<float> kSkyboxVertices = {
            -1.0f, -1.0f, -1.0f,    // 0
            -1.0f, -1.0f, 1.0f,     // 1
            -1.0f, 1.0f, -1.0f,     // 2
            -1.0f, 1.0f, 1.0f,      // 3
            1.0f, -1.0f, -1.0f,     // 4
            1.0f, -1.0f, 1.0f,      // 5
            1.0f, 1.0f, -1.0f,      // 6
            1.0f, 1.0f, 1.0f        // 7
        };

        const std::vector<uint32_t> kSkyboxIndices = {
            2, 0, 4, 4, 6, 2,
            1, 0, 2, 2, 3, 1,
            4, 5, 7, 7, 6, 4,
            1, 3, 7, 7, 5, 1,
            2, 6, 7, 7, 3, 2,
            0, 1, 4, 4, 1, 5
        };

        vd::time::TimeManagerPtr    m_pTimeManager;
        vd::fog::FogManagerPtr      m_pFogManager;        
    };
    typedef std::shared_ptr<Sky>	SkyPtr;
}

#endif // VDGE_SKY_HPP

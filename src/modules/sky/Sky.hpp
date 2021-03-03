//
// Created by Vali on 2/17/2021.
//

#ifndef VDGE_SKY_HPP
#define VDGE_SKY_HPP

#include <engine/object/Entity3D.hpp>

#include <engine/loader/PropertiesLoader.hpp>
#include <engine/time/Time.hpp>

#include <engine/injector/Injectable.hpp>
#include <engine/time/TimeManager.hpp>

#include <memory>

namespace mod::sky {
    class Sky : public vd::object::Entity3D, public vd::injector::Injectable {
    public:
        explicit Sky(const std::string& propsFilePath);

        void Link() override;
        void Setup() override;
        void Update() override;

        [[nodiscard]] const glm::vec3& Color() const;
        [[nodiscard]] const glm::vec3& ColorFactor() const;
    private:
        size_t StateAtAngle(float angle);

        glm::vec3   m_CurrentColor;
        glm::vec3   m_CurrentFactor;
        size_t      m_CurrentState;
        float       m_NextSwitch;
        float       m_LastAngle;
        bool        m_WaitReset;

        struct State {
            std::string Name;
            glm::vec3   Color;
            glm::vec3   ColorFactor;
            float       StartAtAngle;
            float       EndAtAngle;
            struct {
                bool    Enable;
                float   StartAtAngle;
                float   MidAtAngle;
                float   EndAtAngle;
            } Mixable;
        };
        std::vector<State>      m_States;

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

    };
    typedef std::shared_ptr<Sky>	SkyPtr;
}

#endif // VDGE_SKY_HPP

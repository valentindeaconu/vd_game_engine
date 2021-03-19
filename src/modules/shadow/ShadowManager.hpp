//
// Created by Vali on 8/28/2020.
//

#ifndef VD_GAME_ENGINE_SHADOWMANAGER_HPP
#define VD_GAME_ENGINE_SHADOWMANAGER_HPP

#include <engine/component/IManager.hpp>

#include <engine/api/gl/Texture.hpp>

#include <engine/injector/CreateAndStore.hpp>
#include <engine/injector/Injectable.hpp>
#include <engine/light/LightManager.hpp>

#include <engine/api/gl/FrameBuffer.hpp>

#include <engine/property/Properties.hpp>
#include <engine/loader/PropertiesLoader.hpp>

#include <memory>

#include "ShadowBox.hpp"

namespace mod::shadow {
    class ShadowShader;
    typedef std::shared_ptr<ShadowShader>   ShadowShaderPtr;

    class ShadowManager : public vd::component::IManager, public vd::injector::Injectable {
    public:
        explicit ShadowManager(const std::string& propsFilePath);

        void Link() override;

        void Init() override;
        void Update() override;
        void CleanUp() override;

        [[nodiscard]] float Distance() const;
        [[nodiscard]] float TransitionDistance() const;

        [[nodiscard]] const vd::gl::FrameBufferPtr& FrameBuffer() const;

        [[nodiscard]] const vd::gl::Texture2DPtr& ShadowTexture() const;

        [[nodiscard]] const glm::mat4& ViewMatrix() const;
        [[nodiscard]] const glm::mat4& ProjectionMatrix() const;
    private:
        void UpdateView();
        void UpdateProjection();

        int m_MapSize;

        float m_Distance;
        float m_TransitionDistance;
        float m_Offset;

        vd::gl::FrameBufferPtr m_pFrameBuffer;

        ShadowBoxPtr    m_pShadowBox;
        ShadowShaderPtr m_pShader;    

        vd::light::LightPtr m_pSun;

        std::shared_ptr<glm::mat4> m_pView;
        std::shared_ptr<glm::mat4> m_pProjection;
    };
    typedef std::shared_ptr<ShadowManager>  ShadowManagerPtr;
}


#endif //VD_GAME_ENGINE_SHADOWMANAGER_HPP

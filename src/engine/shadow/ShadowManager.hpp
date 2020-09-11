//
// Created by Vali on 8/28/2020.
//

#ifndef VD_GAME_ENGINE_SHADOWMANAGER_HPP
#define VD_GAME_ENGINE_SHADOWMANAGER_HPP

#include <engine/foundation/GL.hpp>
#include <engine/model/Texture.hpp>

#include <engine/core/Window.hpp>
#include <engine/core/Camera.hpp>

#include <memory>

#include "ShadowBox.hpp"

namespace vd::shadow {
    class ShadowManager {
    public:
        ShadowManager();
        ~ShadowManager();

        void init(const core::WindowPtr& windowPtr,
                  const core::CameraPtr& cameraPtr,
                  int mapSize,
                  float distance,
                  float transition,
                  float offset);
        void update(const model::LightPtr& sunPtr);
        void cleanUp();

        void bindFramebuffer() const;
        void unbindFramebuffer() const;

        [[nodiscard]] float getDistance() const;
        [[nodiscard]] float getTransitionDistance() const;

        model::ShadowTexturePtr& getShadowTexture();
        [[nodiscard]] const model::ShadowTexturePtr& getShadowTexture() const;

        [[nodiscard]] const glm::mat4& getViewMatrix() const;
        [[nodiscard]] const glm::mat4& getProjectionMatrix() const;
    private:
        void updateLightView(const model::LightPtr& sunPtr);
        void updateProjection();

        int mapSize;

        float distance;
        float transitionDistance;

        GLuint fboId;
        model::ShadowTexturePtr shadowMapPtr;

        ShadowBoxPtr shadowBoxPtr;

        std::shared_ptr<glm::mat4> lightViewPtr;
        std::shared_ptr<glm::mat4> projectionPtr;
    };
    typedef std::shared_ptr<ShadowManager>  ShadowManagerPtr;
}


#endif //VD_GAME_ENGINE_SHADOWMANAGER_HPP

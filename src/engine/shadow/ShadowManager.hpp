//
// Created by Vali on 8/28/2020.
//

#ifndef VD_GAME_ENGINE_SHADOWMANAGER_HPP
#define VD_GAME_ENGINE_SHADOWMANAGER_HPP

#include <engine/foundation/GL.hpp>
#include <engine/model/Texture.hpp>

#include <engine/core/Window.hpp>
#include <engine/core/Camera.hpp>

#include <engine/glmodel/buffer/FrameBuffer.hpp>

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
        void update(const light::LightPtr& sunPtr);
        void cleanUp();

        [[nodiscard]] float getDistance() const;
        [[nodiscard]] float getTransitionDistance() const;

        [[nodiscard]] const buffer::FrameBufferPtr& getFramebuffer() const;

        [[nodiscard]] const model::Texture2DPtr& getShadowTexture() const;

        [[nodiscard]] const glm::mat4& getViewMatrix() const;
        [[nodiscard]] const glm::mat4& getProjectionMatrix() const;
    private:
        void updateLightView(const light::LightPtr& sunPtr);
        void updateProjection();

        int mapSize;

        float distance;
        float transitionDistance;

        buffer::FrameBufferPtr frameBufferPtr;

        ShadowBoxPtr shadowBoxPtr;

        std::shared_ptr<glm::mat4> lightViewPtr;
        std::shared_ptr<glm::mat4> projectionPtr;
    };
    typedef std::shared_ptr<ShadowManager>  ShadowManagerPtr;
}


#endif //VD_GAME_ENGINE_SHADOWMANAGER_HPP

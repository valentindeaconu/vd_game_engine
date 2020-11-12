//
// Created by Vali on 8/28/2020.
//

#ifndef VD_GAME_ENGINE_SHADOWMANAGER_HPP
#define VD_GAME_ENGINE_SHADOWMANAGER_HPP

#include <engine/foundation/GL.hpp>

#include <engine/component/IManager.hpp>

#include <engine/model/Texture.hpp>

#include <engine/misc/ObjectOfType.hpp>
#include <engine/misc/Properties.hpp>
#include <engine/light/LightManager.hpp>

#include <engine/glmodel/buffer/FrameBuffer.hpp>

#include <memory>

#include "ShadowBox.hpp"

namespace vd::shadow {
    class ShadowManager : public component::IManager {
    public:
        ShadowManager();
        ~ShadowManager();

        void Init() override;
        void Update() override;
        void CleanUp() override;

        [[nodiscard]] float Distance() const;
        [[nodiscard]] float TransitionDistance() const;

        [[nodiscard]] const buffer::FrameBufferPtr& FrameBuffer() const;

        [[nodiscard]] const model::Texture2DPtr& ShadowTexture() const;

        [[nodiscard]] const glm::mat4& ViewMatrix() const;
        [[nodiscard]] const glm::mat4& ProjectionMatrix() const;
    private:
        void UpdateView();
        void UpdateProjection();

        int m_MapSize;

        float m_Distance;
        float m_TransitionDistance;

        buffer::FrameBufferPtr m_FrameBufferPtr;

        ShadowBoxPtr m_ShadowBoxPtr;

        vd::light::LightPtr m_SunPtr;

        std::shared_ptr<glm::mat4> m_ViewPtr;
        std::shared_ptr<glm::mat4> m_ProjectionPtr;
    };
    typedef std::shared_ptr<ShadowManager>  ShadowManagerPtr;
}


#endif //VD_GAME_ENGINE_SHADOWMANAGER_HPP

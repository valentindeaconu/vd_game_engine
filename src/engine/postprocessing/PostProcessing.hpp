//
// Created by Vali on 2/19/2021.
//

#ifndef VDGE_POSTPROCESSING_HPP
#define VDGE_POSTPROCESSING_HPP

#include <memory>

#include <engine/api/gl/Buffer.hpp>
#include <engine/api/gl/Shader.hpp>

#include <engine/injector/Injectable.hpp>
#include <engine/kernel/Context.hpp>
#include <engine/window/Window.hpp>

#include "Stage.hpp"
#include "IStageShader.hpp"

namespace vd::postprocessing {
    class PostProcessing : public injector::Injectable {
    public:
        void Link() override;

        void Init();
        void Render();
        void CleanUp();

        void PushStage(const StagePtr& stage, const IStageShaderPtr& shader);
        void RemoveStage(const std::string& name);
    private:
        const glm::vec2 kQuadVertices[4] = {
                glm::vec2(-1.0f, 1.0f),
                glm::vec2(-1.0f, -1.0f),
                glm::vec2(1.0f, 1.0f),
                glm::vec2(1.0f, -1.0f)
        };
        const GLuint kQuadIndices[6] = { 0, 1, 2, 2, 1, 3 };
        gl::BufferPtr m_pQuadBuffer;

        struct StageShader {
            StagePtr Stage;
            IStageShaderPtr Shader;
        };
        std::vector<StageShader> m_Stages;

        gl::ShaderPtr       m_pToScreenShader;
        kernel::ContextPtr  m_pContext;
        window::WindowPtr   m_pWindow;
    };
    typedef std::shared_ptr<PostProcessing>    PostProcessingPtr;
}


#endif //VDGE_POSTPROCESSING_HPP

//
// Created by Vali on 2/19/2021.
//

#include "PipelineManager.hpp"

namespace vd::postprocessing {


    void PipelineManager::Init() {
        // Create screen quad
        m_pQuadBuffer = std::make_shared<gl::Buffer>();
        m_pQuadBuffer->Bind();
        m_pQuadBuffer->AddBuffer(gl::buffer::eArrayBuffer,
                                 8 * sizeof(float),
                                 kQuadVertices,
                                 gl::buffer::eStaticDraw);
        m_pQuadBuffer->AttributeArray(0, 2, vd::gl::eFloat, 2 * sizeof(float), nullptr);
        m_pQuadBuffer->Unbind();

        for (auto& s : m_Stages) {
            s.second.Stage->Init();
        }
    }

    void PipelineManager::Render() {
        m_pQuadBuffer->Bind();
        size_t count = 0;
        for (auto& p : m_Stages) {
            StagePtr& stage = p.second.Stage;
            IStageShaderPtr& shader = p.second.Shader;

            if (stage->Precondition()) {
                stage->Prepare();

                if (count != m_Stages.size() - 1) {
                    stage->FrameBuffer()->Bind();
                }

                glClear(GL_COLOR_BUFFER_BIT);
                m_pQuadBuffer->DrawArrays(gl::eTriangleStrip, 4);

                if (count != m_Stages.size() - 1) {
                    stage->FrameBuffer()->Unbind();
                }

                stage->Finish();
            }

            ++count;
        }
        m_pQuadBuffer->Unbind();
    }

    void PipelineManager::CleanUp() {
        for (auto& s : m_Stages) {
            s.second.Stage->CleanUp();
        }
    }

    void PipelineManager::PushStage(const StagePtr& stage, const IStageShaderPtr& shader) {
        if (m_Stages.contains(stage->Name())) {
            throw RuntimeError("a stage with name " + stage->Name() + " already exists");
        }

        m_Stages[stage->Name()] = { stage, shader };
    }

    void PipelineManager::RemoveStage(const std::string& name) {
        auto it = m_Stages.find(name);
        if (it != m_Stages.end()) {
            m_Stages.erase(it);
        }
    }
}
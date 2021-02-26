//
// Created by Vali on 2/19/2021.
//

#ifndef VDGE_PIPELINEMANAGER_HPP
#define VDGE_PIPELINEMANAGER_HPP

#include <memory>

#include <engine/api/gl/Buffer.hpp>

#include "Stage.hpp"
#include "IStageShader.hpp"

namespace vd::postprocessing {
    class PipelineManager {
    public:
        void Init();
        void Render();
        void CleanUp();

        void PushStage(const StagePtr& stage, const IStageShaderPtr& shader);
        void RemoveStage(const std::string& name);
    private:
        const float kQuadVertices[8] = { -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f };
        gl::BufferPtr m_pQuadBuffer;

        struct StageShader {
            StagePtr Stage;
            IStageShaderPtr Shader;
        };
        std::unordered_map<std::string, StageShader> m_Stages;
    };
    typedef std::shared_ptr<PipelineManager>    PipelineManagerPtr;
}


#endif //VDGE_PIPELINEMANAGER_HPP

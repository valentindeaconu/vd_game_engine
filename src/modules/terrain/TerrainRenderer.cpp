//
// Created by Vali on 9/21/2020.
//

#include "TerrainRenderer.hpp"

namespace mod::terrain {

    TerrainRenderer::TerrainRenderer(TerrainPtr terrainPtr,
                                     vd::shader::ShaderPtr shaderPtr,
                                     vd::Consumer beforeExecution,
                                     vd::Consumer afterExecution)
        : IRenderer(std::move(shaderPtr), std::move(beforeExecution), std::move(afterExecution))
        , m_TerrainPtr(std::move(terrainPtr))
    {
    }

    TerrainRenderer::~TerrainRenderer() = default;

    void TerrainRenderer::Init() {
        m_TerrainPtr->Init();
    }

    void TerrainRenderer::Update() {
        m_TerrainPtr->Update();
    }

    void TerrainRenderer::Render(const params_t& params) {
        if (!IsReady()) {
            vd::Logger::warn("TerrainRenderer was not ready to render");
            return;
        }

        const auto& renderingPass = params.at("RenderingPass");
        if (renderingPass == "Shadow") {
            return;
        }

        Prepare();

        m_ShaderPtr->bind();

        const auto& rootNodes = m_TerrainPtr->GetRootNodes();

        for (auto& rootNode : rootNodes) {
            RenderNode(rootNode);
        }

        Finish();
    }

    void TerrainRenderer::RenderNode(const TerrainNode::ptr_type_t& nodePtr) {
        if (nodePtr != nullptr) {
            if (nodePtr->IsLeaf()) {
                m_ShaderPtr->bind();

                m_ShaderPtr->setUniform("localModel", nodePtr->GetTransform().Get());
                m_ShaderPtr->setUniform("tessFactor", nodePtr->GetTessFactors());

                m_ShaderPtr->updateUniforms(m_TerrainPtr, 0);

                vd::buffer::BufferPtrVec& buffers = m_TerrainPtr->Buffers();
                buffers[0]->Render();
            } else {
                const auto& children = nodePtr->GetChildren();
                for (const auto& child : children) {
                    RenderNode(std::dynamic_pointer_cast<TerrainNode>(child));
                }
            }
        }
    }

    void TerrainRenderer::CleanUp() {
        m_TerrainPtr->CleanUp();
    }

    bool TerrainRenderer::IsReady() {
        return IRenderer::IsReady() && m_TerrainPtr != nullptr;
    }
}
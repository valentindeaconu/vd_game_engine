//
// Created by Vali on 9/21/2020.
//

#include "TerrainRenderer.hpp"

namespace mod::terrain {

    TerrainRenderer::TerrainRenderer()
        : vd::component::Renderer()
        , m_TerrainPtr(nullptr)
    {
    }

    TerrainRenderer::~TerrainRenderer() = default;

    void TerrainRenderer::Init() {
        m_TerrainPtr->Init();
    }

    void TerrainRenderer::Update() {
        m_TerrainPtr->Update();
    }

    void TerrainRenderer::Render(const vd::kernel::RenderingPass& renderingPass) {
        if (IsReady() && renderingPass != vd::kernel::eShadow) {
            if (m_ConfigPtr != nullptr) {
                m_ConfigPtr->enable();
            }

            const auto& rootNodes = m_TerrainPtr->GetRootNodes();

            for (auto& rootNode : rootNodes) {
                renderNode(rootNode);
            }

            if (m_ConfigPtr != nullptr) {
                m_ConfigPtr->disable();
            }
        }
    }

    void TerrainRenderer::renderNode(const TerrainNode::ptr_type_t& nodePtr) {
        if (nodePtr != nullptr) {
            if (nodePtr->IsLeaf()) {
                m_ShaderPtr->bind();

                m_ShaderPtr->setUniform("localModel", nodePtr->GetTransform().Get());
                m_ShaderPtr->setUniform("tessFactor", nodePtr->GetTessFactors());

                m_ShaderPtr->updateUniforms(m_TerrainPtr, 0);

                vd::buffer::BufferPtrVec& buffers = m_TerrainPtr->GetBuffers();
                buffers[0]->Render();
            } else {
                const auto& children = nodePtr->GetChildren();
                for (const auto& child : children) {
                    renderNode(std::dynamic_pointer_cast<TerrainNode>(child));
                }
            }
        }
    }

    void TerrainRenderer::CleanUp() {
        m_TerrainPtr->CleanUp();
    }

    TerrainPtr& TerrainRenderer::GetTerrain() {
        return m_TerrainPtr;
    }

    const TerrainPtr& TerrainRenderer::GetTerrain() const {
        return m_TerrainPtr;
    }

    void TerrainRenderer::SetTerrain(const TerrainPtr& terrainPtr) {
        this->m_TerrainPtr = terrainPtr;
    }

    bool TerrainRenderer::IsReady() {
        return Renderer::IsReady() && m_TerrainPtr != nullptr;
    }
}
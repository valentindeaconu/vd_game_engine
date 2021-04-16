//
// Created by Vali on 9/21/2020.
//

#include "TerrainRenderer.hpp"

namespace mod::terrain {

    TerrainRenderer::TerrainRenderer(TerrainPtr terrain, vd::component::IEntityShaderPtr shader)
        : IRenderer("TerrainRenderer")
        , m_pShader(std::move(shader))
        , m_pTerrain(std::move(terrain))
    {
    }

    void TerrainRenderer::Link() {
        m_pFrustumCullingManager = vd::ObjectOfType<vd::culling::FrustumCullingManager>::Find();
    }

    void TerrainRenderer::OnInit() {
        m_pTerrain->Init();

        m_pShader->Init();
        m_pShader->Bind();
        m_pShader->InitUniforms(m_pTerrain);
        m_pShader->Unbind();
    }

    void TerrainRenderer::OnUpdate() {
        m_pTerrain->Update();

        m_BufferData.clear();
        m_BufferData.reserve(m_pTerrain->MaximumDataLength());
        m_LeafCount = 0;

        const auto& rootNodes = m_pTerrain->RootNodes();
        for (auto& rootNode : rootNodes) {
            CollectData(rootNode, m_BufferData, m_LeafCount);
        }
    }

    void TerrainRenderer::OnRender(const params_t& params) {
        m_pShader->Bind();
        m_pShader->UpdateUniforms(m_pTerrain, 0, 0);

        vd::gl::wrappers::BufferPtr& buffer = m_pTerrain->Buffers()[0];
        buffer->UpdateBufferData(vd::gl::eArrayBuffer, (m_BufferData.size() << 2), &m_BufferData[0], 1);
        buffer->DrawArraysInstanced(vd::gl::ePatches, 16, m_LeafCount);

        m_pShader->Unbind();
    }

    void TerrainRenderer::CollectData(const TerrainNode::ptr_type_t& pNode, std::vector<float>& data, size_t& leafCount) {
        if (pNode != nullptr) {
            using namespace vd::collision;
        
            if (Detector::Bounds3AgainstFrustum(pNode->Bounds(), m_pFrustumCullingManager->Frustum()) != eOutside) {
                if (pNode->Leaf()) {
                    glm::mat4 localModel = pNode->Transform().Get();

                    for (size_t i = 0; i < 4; ++i) {
                        for (size_t j = 0; j < 4; ++j) {
                            data.emplace_back(localModel[i][j]);
                        }
                    }

                    const glm::vec4& tessFactor = pNode->TessFactors();

                    for (size_t i = 0; i < 4; ++i) {
                        data.emplace_back(tessFactor[i]);
                    }

                    leafCount++;
                } else {
                    const auto& children = pNode->Children();
                    for (const auto& child : children) {
                        CollectData(std::dynamic_pointer_cast<TerrainNode>(child), data, leafCount);
                    }
                }
            }
        }
    }

    void TerrainRenderer::OnCleanUp() {
        m_pTerrain->CleanUp();
        m_pShader->CleanUp();
    }

    bool TerrainRenderer::Precondition(const params_t& params) {
        if (m_pShader == nullptr || m_pTerrain == nullptr) {
            return false;
        }

        const auto& renderingPass = params.at("RenderingPass");
        return (renderingPass == "Reflection" ||
                renderingPass == "Refraction" ||
                renderingPass == "Main");
    }

    void TerrainRenderer::Prepare() {
        vd::gl::Context::CounterClockwiseFacing();
    }

    void TerrainRenderer::Finish() {
        vd::gl::Context::Reset();
    }
}
//
// Created by Vali on 9/21/2020.
//

#include "TerrainRenderer.hpp"

namespace mod::terrain {

    TerrainRenderer::TerrainRenderer(TerrainPtr terrainPtr,
                                     vd::component::IEntityShaderPtr shaderPtr,
                                     vd::Consumer beforeExecution,
                                     vd::Consumer afterExecution)
        : IRenderer(std::move(shaderPtr), std::move(beforeExecution), std::move(afterExecution))
        , m_pTerrain(std::move(terrainPtr))
    {
    }

    void TerrainRenderer::Link() {
        m_pFrustumCullingManager = vd::ObjectOfType<vd::culling::FrustumCullingManager>::Find();
    }

    void TerrainRenderer::Init() {
        m_pTerrain->Init();

        m_pShader->Init();
        m_pShader->Bind();
        m_pShader->InitUniforms(m_pTerrain);
        m_pShader->Unbind();
    }

    void TerrainRenderer::Update() {
        m_pTerrain->Update();

        m_BufferData.clear();
        m_BufferData.reserve(m_pTerrain->MaximumDataLength());
        m_LeafCount = 0;

        const auto& rootNodes = m_pTerrain->RootNodes();
        for (auto& rootNode : rootNodes) {
            CollectData(rootNode, m_BufferData, m_LeafCount);
        }
    }

    void TerrainRenderer::Render(const params_t& params) {
        if (!IsReady()) {
            vd::Logger::warn("TerrainRenderer was not ready to render");
            return;
        }

        const auto& renderingPass = params.at("RenderingPass");

        if (renderingPass == "Reflection" ||
            renderingPass == "Refraction" ||
            renderingPass == "Main") {
            Prepare();

            m_pShader->Bind();
            m_pShader->UpdateUniforms(m_pTerrain, 0, 0);

            /*const auto& rootNodes = m_pTerrain->RootNodes();
            for (auto& rootNode : rootNodes) {
                CollectData(rootNode, m_BufferData, m_LeafCount);
            }*/

            vd::gl::BufferPtr& buffer = m_pTerrain->Buffers()[0];
            buffer->UpdateBufferData(vd::gl::eArrayBuffer, (m_BufferData.size() << 2), &m_BufferData[0], 1);
            buffer->DrawArraysInstanced(vd::gl::ePatches, 16, m_LeafCount);

            m_pShader->Unbind();

            Finish();
        }
    }

    void TerrainRenderer::CollectData(const TerrainNode::ptr_type_t& pNode, std::vector<float>& data, size_t& leafCount) {
        if (pNode != nullptr) {
            using namespace vd::collision;
        
            if (Detector::Bounds3AgainstFrustum(pNode->Bounds(), m_pFrustumCullingManager->Frustum()) != eOutside) {
                if (pNode->Leaf()) {
                    /*m_pShader->PushUniform("vLocalModel", pNode->Transform().Get());
                    m_pShader->PushUniform("vTessFactor", pNode->TessFactors());

                    vd::gl::BufferPtr& buffer = m_pTerrain->Buffers()[0];
                    buffer->DrawArrays(vd::gl::ePatches, 16);*/

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

    void TerrainRenderer::CleanUp() {
        m_pTerrain->CleanUp();
        m_pShader->CleanUp();
    }

    bool TerrainRenderer::IsReady() {
        return IRenderer::IsReady() && m_pTerrain != nullptr;
    }
}
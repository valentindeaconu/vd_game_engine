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

    TerrainRenderer::~TerrainRenderer() = default;

    void TerrainRenderer::Link() {
        m_pFrustumCullingManager = vd::ObjectOfType<vd::culling::FrustumCullingManager>::Find();
    }

    void TerrainRenderer::Init() {
        m_pTerrain->Init();

        m_pShader->Bind();
        m_pShader->InitUniforms(m_pTerrain);
    }

    void TerrainRenderer::Update() {
        m_pTerrain->Update();
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

            const auto& rootNodes = m_pTerrain->RootNodes();

            for (auto &rootNode : rootNodes) {
                RenderNode(rootNode);
            }

            Finish();
        }
     }

    void TerrainRenderer::RenderNode(const TerrainNode::ptr_type_t& pNode) {
        if (pNode != nullptr) {
            using namespace vd::collision;

            if (Detector::Bounds3AgainstFrustum(pNode->Bounds(), m_pFrustumCullingManager->Frustum()) != eOutside) {
                if (pNode->Leaf()) {
                    m_pShader->Bind();

                    m_pShader->SetUniform("localModel", pNode->Transform().Get());
                    m_pShader->SetUniform("tessFactor", pNode->TessFactors());

                    m_pShader->UpdateUniforms(m_pTerrain, 0, 0);

                    vd::gl::BufferPtr& buffer = m_pTerrain->Buffers()[0];
                    buffer->DrawArrays(vd::gl::ePatches, 16);
                } else {
                    const auto& children = pNode->Children();
                    for (const auto& child : children) {
                        RenderNode(std::dynamic_pointer_cast<TerrainNode>(child));
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
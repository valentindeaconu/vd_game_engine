//
// Created by vali on 4/15/21.
//

#include "IRenderer.hpp"

namespace vd::gapi {

    void IRenderer::Render() {
        while (!m_ActionsQueue.empty()) {
            auto& [Action, Callback] = m_ActionsQueue.front();
            m_ActionsQueue.pop();

            Action();
            Callback();
        }
    }


    void IRenderer::Submit(const model::MaterialMesh& mesh, const math::Transform& transform) {
        auto pair = std::make_pair([=]() {
            m_Context.Program->Select();
            m_Context.Program->Update(mesh, transform);

            mesh->Buffer()->DrawElements(vd::gl::eTriangles, mesh.Indices().size(), vd::gl::eUnsignedInt);

            m_Context.Program->Unselect();
        }, vd::g_kEmptyConsumer);
    }

    void IRenderer::SetView(const glm::mat4& view) {
        m_Context.View = view;
    }

    void IRenderer::SetProjection(const glm::mat4& projection) {
        m_Context.Projection = projection;
    }

    void IRenderer::SetProgram(IProgramPtr program) {
        m_Context.Program = std::move(program);
    }

    void IRenderer::UpdateProgram(const IProgramPtr& program, const Storage& storage) {
        for (const auto& pair : storage) {
            if (!program->Set(pair.first, pair.second)) {
                Logger::log("Could not set parameter: " + pair.first);
            }
        }
    }

}

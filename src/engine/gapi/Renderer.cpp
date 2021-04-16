//
// Created by vali on 4/15/21.
//

#include "Renderer.hpp"

namespace vd::gapi {

    void Renderer::Render() {
        while (!m_ActionsQueue.empty()) {
            auto& [Action, Callback] = m_ActionsQueue.front();
            m_ActionsQueue.pop();

            Action();
            Callback();
        }
    }

    void Renderer::Submit(const model::Mesh& mesh, const math::Transform& transform, Storage& programParameters) {
        programParameters["uModel"] = transform.Get();
        programParameters["uView"] = m_Context.View;
        programParameters["uProjection"] = m_Context.Projection;

        auto pair = std::make_pair([program = m_Context.Program, mesh = mesh]() {
            program->Use();

            // TODO: What to do with the "Storage"?
            UpdateProgram(program, Storage());

            mesh.Draw();

            program->Discard();
        }, vd::g_kEmptyConsumer);

        m_ActionsQueue.push(std::move(pair));
    }

    void Renderer::SetView(const glm::mat4& view) {
        m_Context.View = view;
    }

    void Renderer::SetProjection(const glm::mat4& projection) {
        m_Context.Projection = projection;
    }

    void Renderer::SetProgram(ProgramPtr program) {
        m_Context.Program = std::move(program);
    }

    void Renderer::UpdateProgram(const ProgramPtr& program, const Storage& storage) {
        for (const auto& pair : storage) {
            if (!program->Set(pair.first, pair.second)) {
                Logger::log("Could not set parameter: " + pair.first);
            }
        }
    }

}

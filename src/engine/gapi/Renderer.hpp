//
// Created by vali on 4/15/21.
//

#ifndef VDGE_RENDERER_HPP
#define VDGE_RENDERER_HPP

#include <queue>

#include <engine/math/Transform.hpp>
#include <engine/model/Mesh.hpp>

#include "Program.hpp"

namespace vd::gapi {

    class Renderer {
    public:
        void Render();

        void Submit(const model::Mesh&, const math::Transform&, Storage&);

        void SetView(const glm::mat4& view);
        void SetProjection(const glm::mat4& projection);
        void SetProgram(ProgramPtr program);
    private:
        static void UpdateProgram(const ProgramPtr& program, const Storage& storage);

        struct RendererContext {
            glm::mat4   View;
            glm::mat4   Projection;
            ProgramPtr  Program;
        } m_Context;

        std::queue<std::pair<vd::Consumer, vd::Consumer>>   m_ActionsQueue;
    };

}

#endif //VDGE_RENDERER_HPP

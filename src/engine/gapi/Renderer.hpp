//
// Created by vali on 4/15/21.
//

#ifndef VDGE_IRENDERER_HPP
#define VDGE_IRENDERER_HPP

#include <queue>

#include <engine/math/Transform.hpp>
#include <engine/model/Mesh.hpp>

#include "IProgram.hpp"

namespace vd::gapi {

    class IRenderer {
    public:
        void Render();

        void Submit(const model::MaterialMesh&, const math::Transform&);

        void SetView(const glm::mat4& view);
        void SetProjection(const glm::mat4& projection);
        void SetProgram(IProgramPtr program);
    private:
        static void UpdateProgram(const IProgramPtr& program, const Storage& storage);

        struct RendererContext {
            glm::mat4   View;
            glm::mat4   Projection;
            IProgramPtr Program;
        } m_Context;

        std::queue<std::pair<vd::Consumer, vd::Consumer>>   m_ActionsQueue;
    };

}

#endif //VDGE_IRENDERER_HPP

//
// Created by Vali on 4/13/2021.
//

#ifndef VDGE_SCENE_HPP
#define VDGE_SCENE_HPP

#include <memory>

#include <entt.hpp>

#include <engine/defines/RenderingPasses.hpp>
#include <engine/defines/Types.hpp>

#include "Components.hpp"

namespace vd::scene {
    class Entity;

    class Scene {
    public:
        Scene() = default;

        Entity CreateEntity();

        void OnInit();
        void OnUpdate(Timestep ts);
        void OnRender(const RenderingPassType& renderingPass);
        void OnCleanUp();
    private:
        entt::registry m_Registry;

        friend class Entity;
    };
    typedef std::shared_ptr<Scene>  ScenePtr;

}

#endif //VDGE_SCENE_HPP

//
// Created by Vali on 4/13/2021.
//

#include "Scene.hpp"

#include "Entity.hpp"

namespace vd::scene {


    Scene::Scene() {

    }

    Entity Scene::CreateEntity() {
        return { m_Registry.create(), this };
    }

    void Scene::OnInit() {

    }

    void Scene::OnUpdate(Timestep ts) {

    }
    
    void Scene::OnRender(const RenderingPassType& renderingPass) {

    }
    
    void Scene::OnCleanUp() {
        
    }

}
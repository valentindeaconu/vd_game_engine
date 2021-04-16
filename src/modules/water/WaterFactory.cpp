//
// Created by Vali on 11/19/2020.
//

#include "WaterFactory.hpp"

namespace mod::water {

    void WaterFactory::Create(const vd::EnginePtr &pEngine) {
        WaterPtr pWater = vd::injector::CreateAndStore<Water>("./resources/properties/water.properties");
        WaterShaderPtr pWaterShader = std::make_shared<WaterShader>();

        WaterRendererPtr pWaterRenderer = std::make_shared<WaterRenderer>(pWater, pWaterShader);

        vd::camera::CameraPtr pCamera = vd::ObjectOfType<vd::camera::Camera>::Find();
        vd::context::ContextPtr pContext = vd::ObjectOfType<vd::context::Context>::Find();

        vd::Predicate passPrecondition = [w = pWater.get(), c = pCamera.get(), ctx = pContext]() {
            return !ctx->WireframeMode() && c->Position().y > w->GetHeight();
        };

        vd::Consumer reflectionPassBefore = [w = pWater.get(), ctx = pContext.get(), c = pCamera.get()] {
            ctx->ClipPlane() = glm::vec4(0.0f, 1.0f, 0.0f, -w->GetHeight() - 0.5f);
            glEnable(GL_CLIP_DISTANCE0);

            c->Reflect(vd::camera::Camera::eY, w->GetHeight());
        };

        vd::Consumer reflectionPassAfter = [w = pWater.get(), ctx = pContext.get(), c = pCamera.get()] {
            ctx->ClipPlane() = glm::vec4(0.0f);
            glDisable(GL_CLIP_DISTANCE0);

            c->Reflect(vd::camera::Camera::eY, w->GetHeight());
        };

        vd::component::RenderingPass reflectionPass(
                "Reflection",
                20,
                pWater->ReflectionFramebuffer(),
                passPrecondition,
                reflectionPassBefore,
                reflectionPassAfter
        );
        pEngine->Add(reflectionPass);

        vd::Consumer refractionPassBefore = [w = pWater.get(), ctx = pContext.get()] {
            ctx->ClipPlane() = glm::vec4(0.0f, -1.0f, 0.0f, w->GetHeight() + 0.5f);
            glEnable(GL_CLIP_DISTANCE0);
        };

        vd::Consumer refractionPassAfter = [ctx = pContext.get()] {
            ctx->ClipPlane() = glm::vec4(0.0f);
            glDisable(GL_CLIP_DISTANCE0);
        };

        vd::component::RenderingPass refractionPass(
                "Refraction",
                20,
                pWater->RefractionFramebuffer(),
                passPrecondition,
                refractionPassBefore,
                refractionPassAfter
        );

        pEngine->Add(refractionPass);

        /// Water must be the last element to draw, but before GUIs
        pEngine->Subscribe(pWaterRenderer, WaterRenderer::kPriority);
    }

}
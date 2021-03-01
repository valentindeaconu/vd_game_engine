//
// Created by Vali on 2/19/2021.
//

#include "EffectFactory.hpp"

#define CONTEXT_FINDER  vd::ObjectOfType<vd::context::Context>::Find()

namespace mod::postprocessing {

    void EffectFactory::Create(const vd::EnginePtr& pEngine) {
        /// Create & register rendering pass
        vd::component::RenderingPass rpPostProcessing("PostProcessing", 500, nullptr);
        pEngine->Add(rpPostProcessing);

        auto pRenderer = std::make_shared<EffectRenderer>();

        auto defaultConfigurator = [](vd::gl::FrameBufferPtr& pFrameBuffer) {
            pFrameBuffer->Bind();
            pFrameBuffer->PushAttachment(vd::gl::FrameBuffer::eColorTexture, [](vd::gl::Texture2DPtr& pTex) {
                pTex->Bind();
                pTex->BilinearFilter();
                pTex->Unbind();
            });
            pFrameBuffer->Unbind();
        };

        /* /// Horizontal Blur Stage 1 (No downscale)
        auto pHBlur1 = std::make_shared<vd::component::ConcreteEffect>(
            "HorizontalBlur_1",
            0.5f,
            [ctx = CONTEXT_FINDER]() { return vd::gl::FrameBufferPtrVec({ctx->SceneFrameBuffer() }); },
            defaultConfigurator,
            [ctx = CONTEXT_FINDER]() { return !ctx->WireframeMode(); }
        );

        pRenderer->PushStage(pHBlur1, std::make_shared<HorizontalBlurShader>());

        /// Vertical Blur Stage 1 (No downscale)
        auto pVBlur1 = std::make_shared<vd::component::ConcreteEffect>(
            "VerticalBlur_1",
            0.5f,
            [c = pHBlur1]() { return vd::gl::FrameBufferPtrVec({ c->FrameBuffer() }); },
            defaultConfigurator,
            [ctx = CONTEXT_FINDER]() { return !ctx->WireframeMode(); }
        );
        pRenderer->PushStage(pVBlur1, std::make_shared<VerticalBlurShader>());


        /// Horizontal Blur Stage 2 (Downscale x2)
        auto pHBlur2 = std::make_shared<vd::component::ConcreteEffect>(
                "HorizontalBlur_2",
                0.25f,
                [c = pVBlur1]() { return vd::gl::FrameBufferPtrVec({ c->FrameBuffer() }); },
                defaultConfigurator,
                [ctx = CONTEXT_FINDER]() { return !ctx->WireframeMode(); }
        );
        pRenderer->PushStage(pHBlur2, std::make_shared<HorizontalBlurShader>());

        /// Vertical Blur Stage 2 (Downscale x2)
        auto pVBlur2 = std::make_shared<vd::component::ConcreteEffect>(
                "VerticalBlur_2",
                0.25f,
                [c = pHBlur2]() { return vd::gl::FrameBufferPtrVec({ c->FrameBuffer() }); },
                defaultConfigurator,
                [ctx = CONTEXT_FINDER]() { return !ctx->WireframeMode(); }
        );
        pRenderer->PushStage(pVBlur2, std::make_shared<VerticalBlurShader>());

        /// DepthOfField
        auto pDepthOfField = std::make_shared<vd::component::ConcreteEffect>(
                "Depth Of Field",
                1.0f,
                [ctx = CONTEXT_FINDER, i = pVBlur2]() {
                    return vd::gl::FrameBufferPtrVec ({ctx->SceneFrameBuffer(), i->FrameBuffer() });
                },
                defaultConfigurator,
                [ctx = CONTEXT_FINDER]() { return !ctx->WireframeMode(); }
        );
        pRenderer->PushStage(pDepthOfField, std::make_shared<DepthOfFieldShader>()); */

        /// At the end of the processing, send last result to the ToScreenPseudoEffect to display it
        auto pToScreenPseudoEffect = std::make_shared<ToScreenPseudoEffect>(
                [ctx = CONTEXT_FINDER]() { return vd::gl::FrameBufferPtrVec({ ctx->SceneFrameBuffer() }); }
                /*[c = pDepthOfField]() {
                    return vd::gl::FrameBufferPtrVec({ c->FrameBuffer() });
                }*/
        );
        pRenderer->PushStage(pToScreenPseudoEffect, std::make_shared<ToScreenShader>());

        /// Register renderer to the pipeline
        pEngine->Subscribe(pRenderer, EffectRenderer::kPriority);
    }
}

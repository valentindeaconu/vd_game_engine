//
// Created by Vali on 2/19/2021.
//

#include "PostProcessingFactory.hpp"

#define CONTEXT_FINDER  vd::ObjectOfType<vd::kernel::Context>::Find()

namespace mod::postprocessing {

    void PostProcessingFactory::Create(const vd::EnginePtr& pEngine) {
        auto defaultConfigurator = [](vd::gl::FrameBufferPtr& pFrameBuffer) {
            pFrameBuffer->Bind();
            pFrameBuffer->PushAttachment(vd::gl::FrameBuffer::eColorTexture, [](vd::gl::Texture2DPtr& pTex) {
                pTex->Bind();
                pTex->BilinearFilter();
                pTex->Unbind();
            });
            pFrameBuffer->Unbind();
        };

        /// Horizontal Blur Stage 1 (No downscale)
        auto pHBlur1 = std::make_shared<vd::postprocessing::SingularInputStage>(
            "HorizontalBlur_1",
            1.0f,
            [ctx = CONTEXT_FINDER]() { return ctx->SceneFBO(); },
            defaultConfigurator
        );

        pEngine->PostProcessing()->PushStage(pHBlur1, std::make_shared<HorizontalBlurShader>());

        /// Vertical Blur Stage 1 (No downscale)
        auto pVBlur1 = std::make_shared<vd::postprocessing::SingularInputStage>(
            "VerticalBlur_1",
            1.0f,
            [c = pHBlur1]() { return c->InputFrameBuffer(); },
            defaultConfigurator
        );
        pEngine->PostProcessing()->PushStage(pVBlur1, std::make_shared<VerticalBlurShader>());


        /// Horizontal Blur Stage 2 (Downscale x2)
        auto pHBlur2 = std::make_shared<vd::postprocessing::SingularInputStage>(
                "HorizontalBlur_2",
                0.5f,
                [c = pVBlur1]() { return c->InputFrameBuffer(); },
                defaultConfigurator
        );
        pEngine->PostProcessing()->PushStage(pHBlur2, std::make_shared<HorizontalBlurShader>());

        /// Vertical Blur Stage 2 (Downscale x2)
        auto pVBlur2 = std::make_shared<vd::postprocessing::SingularInputStage>(
                "VerticalBlur_2",
                0.5f,
                [c = pHBlur2]() { return c->InputFrameBuffer(); },
                defaultConfigurator
        );
        pEngine->PostProcessing()->PushStage(pVBlur2, std::make_shared<VerticalBlurShader>());

        /// DepthOfField
        auto pDepthOfField = std::make_shared<vd::postprocessing::MultipleInputStage>(
                "Depth Of Field",
                1.0f,
                [ctx = CONTEXT_FINDER, i = pVBlur2]() {
                    return vd::gl::FrameBufferPtrVec ({ ctx->SceneFBO(), i->FrameBuffer() });
                },
                defaultConfigurator
        );
        pEngine->PostProcessing()->PushStage(pDepthOfField, std::make_shared<DepthOfFieldShader>());
    }
}

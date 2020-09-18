#include "EntityRenderer.hpp"

namespace vd::component::impl
{
    EntityRenderer::EntityRenderer()
        : Renderer()
        , entityPtr(nullptr)
    {
    }

    EntityRenderer::~EntityRenderer() = default;

    void EntityRenderer::init() {
        entityPtr->init();
    }

    void EntityRenderer::update() {
        entityPtr->update();
    }

    void EntityRenderer::render(const kernel::RenderingPass& renderingPass) {
        if (isReady()) {
            if (renderConfigPtr != nullptr) {
                renderConfigPtr->enable();
            }

            auto _shaderPtr = renderingPass == vd::kernel::RenderingPass::eShadow ? this->getShadowShader() : shaderPtr;

            _shaderPtr->bind();
            buffer::MeshBufferPtrVec &meshBuffers = entityPtr->getMeshBuffers();
            for (size_t meshIndex = 0; meshIndex < meshBuffers.size(); ++meshIndex) {
                _shaderPtr->updateUniforms(entityPtr, meshIndex);
                meshBuffers[meshIndex]->render();
            }

            if (renderConfigPtr != nullptr) {
                renderConfigPtr->disable();
            }
        }
    }

    void EntityRenderer::cleanUp()
    {
        entityPtr->cleanUp();
    }

    vd::object::EntityPtr& EntityRenderer::getEntity()
    {
        return entityPtr;
    }

    const vd::object::EntityPtr& EntityRenderer::getEntity() const
    {
        return entityPtr;
    }

    void EntityRenderer::setEntity(const vd::object::EntityPtr& entityPtr)
    {
        this->entityPtr = entityPtr;
    }

    bool EntityRenderer::isReady()
    {
        return Renderer::isReady() && entityPtr != nullptr;
    }
}
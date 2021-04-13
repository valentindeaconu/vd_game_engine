#include "IRenderer.hpp"

namespace vd::component {
    namespace exception {
        RendererError::RendererError(const std::string& name, const std::string& message) 
            : CustomException("RendererError", "[" + name + "]: " + message)
        {
        }
    }

    IRenderer::IRenderer(std::string name)
        : m_Initialized(false)
        , m_Name(std::move(name))
    {
    }

    void IRenderer::Init() {
#ifdef VDGE_DEBUG
        if (m_Initialized) {
            throw exception::RendererError(m_Name, "Renderer is already initialized");
        }
#endif
        if (!m_Initialized) {
            OnInit();
            m_Initialized = true;
        }
    }

    void IRenderer::Update() {        
        if (!m_Initialized) {
            throw exception::RendererError(m_Name, "Updating an uninitialized renderer");
        }

        OnUpdate();
    }

    void IRenderer::Render(const params_t& params) {
        if (!m_Initialized) {
            throw exception::RendererError(m_Name, "Rendering an uninitialized renderer");
        }

        if (Precondition(params)) {
            Prepare();

            OnRender(params);

            Finish();
        }
    }

    void IRenderer::CleanUp() {
        if (m_Initialized) {
            OnCleanUp();
            m_Initialized = false;
        }
    }

    const std::string& IRenderer::Name() const {
        return m_Name;
    }

}
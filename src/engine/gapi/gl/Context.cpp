//
// Created by Vali on 3/17/2021.
//

#include "Context.hpp"

namespace vd::gl {

    Context::Context()
        : m_Facing(Facing::eCW)
        , m_Blending(Blending::eNone)
        , m_DepthMaskEnabled(false)
        , m_DepthTesting(DepthTesting::eNone)
        , m_ClearColor(0.0f, 0.0f, 0.0f, 1.0f)
        , m_FramebuffersEnabled(false)
        , m_Culling(Culling::eNone)
        , m_PolygonMode(PolygonMode::eFill)
    {
    }

    void Context::OnCreate() {
        if (s_Context == nullptr) {
            s_Context = this;
        } else {
            throw RuntimeError("GL Context is already initialized");
        }

        // TODO: glewExperimental should be disabled on production binary
        glewExperimental = GL_TRUE;

        if (GLEW_OK != glewInit()) {
            throw RuntimeError("Failed to initialize GLEW");
        }

        Reset();
        Summary();
    }

    void Context::OnCleanUp() {
        
    }

    GLenum Context::CheckError(const char* file, int line) {
        GLenum errorCode;
        while ((errorCode = glGetError()) != GL_NO_ERROR) {
            std::string error;
            switch (errorCode) {
                case GL_INVALID_ENUM:                  error = "Invalid Enum"; break;
                case GL_INVALID_VALUE:                 error = "Invalid Value"; break;
                case GL_INVALID_OPERATION:             error = "Invalid Operation"; break;
                case GL_STACK_OVERFLOW:                error = "Stack Overflow"; break;
                case GL_STACK_UNDERFLOW:               error = "Stack Underflow"; break;
                case GL_OUT_OF_MEMORY:                 error = "Out of memory"; break;
                case GL_INVALID_FRAMEBUFFER_OPERATION: error = "Invalid Framebuffer Operation"; break;
                default:                               continue;
            }

            std::stringstream merr;
            merr << error << " | " << file << " (" << line << ")";           
            Logger::warn(merr.str());
        }
        return errorCode;
    }

    void Context::Reset() {
        CounterClockwiseFacing();
        NoBlending();
        DepthMask(true);
        LessDepthTesting();
        ClearColor(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
        UseFramebuffers(true);
        BackCulling();
        DrawModeFill();        
    }

    void Context::ClockwiseFacing() {
        if (s_Context->m_Facing != Facing::eCW) {
            glFrontFace(GL_CW);
            s_Context->m_Facing = Facing::eCW;
        }
    }

    void Context::CounterClockwiseFacing() {
        if (s_Context->m_Facing != Facing::eCCW) {
            glFrontFace(GL_CCW);
            s_Context->m_Facing = Facing::eCCW;
        }
    }

    void Context::NoBlending() {
        if (s_Context->m_Blending != Blending::eNone) {
            glDisable(GL_BLEND);

            s_Context->m_Blending = Blending::eNone;
        }
    }

    void Context::AlphaBlending() {
        if (s_Context->m_Blending != Blending::eAlpha) {
            if (s_Context->m_Blending == Blending::eNone) {
                glEnable(GL_BLEND);
            }
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            s_Context->m_Blending = Blending::eAlpha;
        }
    }

    void Context::AdditiveBlending() {
        if (s_Context->m_Blending != Blending::eAdditive) {
            if (s_Context->m_Blending == Blending::eNone) {
                glEnable(GL_BLEND);
            }
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);

            s_Context->m_Blending = Blending::eAdditive;
        }
    }

    void Context::DepthMask(bool enabled) {
        if (s_Context->m_DepthMaskEnabled != enabled) {
            glDepthMask(enabled ? GL_TRUE : GL_FALSE);

            s_Context->m_DepthMaskEnabled = enabled;
        }
    }

    void Context::NoDepthTesting() {
        if (s_Context->m_DepthTesting != DepthTesting::eNone) {
            glDisable(GL_DEPTH_TEST);

            s_Context->m_DepthTesting == DepthTesting::eNone;
        }
    }

    void Context::LessDepthTesting() {
        if (s_Context->m_DepthTesting != DepthTesting::eLess) {
            if (s_Context->m_DepthTesting == DepthTesting::eNone) {
                glEnable(GL_DEPTH_TEST);
            }
            glDepthFunc(GL_LESS);

            s_Context->m_DepthTesting == DepthTesting::eLess;
        }
    }

    void Context::EqualDepthTesting() {
        if (s_Context->m_DepthTesting != DepthTesting::eEqual) {
            if (s_Context->m_DepthTesting == DepthTesting::eNone) {
                glEnable(GL_DEPTH_TEST);
            }
            glDepthFunc(GL_EQUAL);

            s_Context->m_DepthTesting == DepthTesting::eEqual;
        }
    }

    void Context::LequalDepthTesting() {
        if (s_Context->m_DepthTesting != DepthTesting::eLequal) {
            if (s_Context->m_DepthTesting == DepthTesting::eNone) {
                glEnable(GL_DEPTH_TEST);
            }
            glDepthFunc(GL_LEQUAL);

            s_Context->m_DepthTesting == DepthTesting::eLequal;
        }
    }

    void Context::GreaterDepthTesting() {
        if (s_Context->m_DepthTesting != DepthTesting::eGreater) {
            if (s_Context->m_DepthTesting == DepthTesting::eNone) {
                glEnable(GL_DEPTH_TEST);
            }
            glDepthFunc(GL_GREATER);

            s_Context->m_DepthTesting == DepthTesting::eGreater;
        }
    }

    void Context::NotEqualDepthTesting() {
        if (s_Context->m_DepthTesting != DepthTesting::eNotEqual) {
            if (s_Context->m_DepthTesting == DepthTesting::eNone) {
                glEnable(GL_DEPTH_TEST);
            }
            glDepthFunc(GL_NOTEQUAL);

            s_Context->m_DepthTesting == DepthTesting::eNotEqual;
        }
    }

    void Context::GequalDepthTesting() {
        if (s_Context->m_DepthTesting != DepthTesting::eGequal) {
            if (s_Context->m_DepthTesting == DepthTesting::eNone) {
                glEnable(GL_DEPTH_TEST);
            }
            glDepthFunc(GL_GEQUAL);

            s_Context->m_DepthTesting == DepthTesting::eGequal;
        }
    }

    void Context::AlwaysDepthTesting() {
        if (s_Context->m_DepthTesting != DepthTesting::eAlways) {
            if (s_Context->m_DepthTesting == DepthTesting::eNone) {
                glEnable(GL_DEPTH_TEST);
            }
            glDepthFunc(GL_ALWAYS);

            s_Context->m_DepthTesting == DepthTesting::eAlways;
        }
    }


    void Context::ClearColor(const glm::vec4& color) {
        if (s_Context->m_ClearColor != color) {
            glClearColor(color.r, color.g, color.b, color.a);
            s_Context->m_ClearColor = color;
        }
    }


    void Context::UseFramebuffers(bool enabled) {
        if (s_Context->m_FramebuffersEnabled != enabled) {
            if (enabled) {
                glEnable(GL_FRAMEBUFFER_SRGB);  
            } else {
                glDisable(GL_FRAMEBUFFER_SRGB);
            }

            s_Context->m_FramebuffersEnabled = enabled;
        }
    }


    void Context::NoCulling() {
        if (s_Context->m_Culling != Culling::eNone) {
            glDisable(GL_CULL_FACE);

            s_Context->m_Culling = Culling::eNone;
        }
    }

    void Context::FrontCulling() {
        if (s_Context->m_Culling != Culling::eFront) {
            if (s_Context->m_Culling != Culling::eNone) {
                glEnable(GL_CULL_FACE);
            }
            glCullFace(GL_FRONT);

            s_Context->m_Culling = Culling::eFront;
        }
    }

    void Context::BackCulling() {
        if (s_Context->m_Culling != Culling::eBack) {
            if (s_Context->m_Culling != Culling::eNone) {
                glEnable(GL_CULL_FACE);
            }
            glCullFace(GL_BACK);

            s_Context->m_Culling = Culling::eBack;
        }
    }

    void Context::FrontAndBackCulling() {
        if (s_Context->m_Culling != Culling::eFrontAndBack) {
            if (s_Context->m_Culling != Culling::eNone) {
                glEnable(GL_CULL_FACE);
            }
            glCullFace(GL_FRONT_AND_BACK);

            s_Context->m_Culling = Culling::eFrontAndBack;
        }
    }

    void Context::DrawModePolygonPoints() {
        if (s_Context->m_PolygonMode != PolygonMode::ePoint) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

            s_Context->m_PolygonMode = PolygonMode::ePoint;
        }
    }

    void Context::DrawModeWireframe() {
        if (s_Context->m_PolygonMode != PolygonMode::eLine) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

            s_Context->m_PolygonMode = PolygonMode::eLine;
        }
    }

    void Context::DrawModeFill() {
        if (s_Context->m_PolygonMode != PolygonMode::eFill) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            s_Context->m_PolygonMode = PolygonMode::eFill;
        }
    }


    void Context::Summary() {
        std::stringstream r;
        r << glGetString(GL_RENDERER);
        Logger::log("Renderer: " + r.str());

        std::stringstream v;
        v << glGetString(GL_VERSION);
        Logger::log("OpenGL Version: " + v.str());
    }

}
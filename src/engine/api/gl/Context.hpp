//
// Created by Vali on 3/17/2021.
//

#ifndef VDGE_GL_CONTEXT_HPP
#define VDGE_GL_CONTEXT_HPP

#include <string>
#include <sstream>
#include <memory>
#include <glm/glm.hpp>

#define GLEW_STATIC
#include <GL/glew.h>

#include <engine/logger/Logger.hpp>
#include <engine/injector/Injectable.hpp>

#include "Component.hpp"

namespace vd::gl {
    class Context : public Component {
    public:
        Context();

        void OnCreate() override;
        void OnCleanUp() override;

        static GLenum CheckError(const char* file, int line);

        // Context methods
        static void Reset();

        static void ClockwiseFacing();
        static void CounterClockwiseFacing();

        static void NoBlending();
        static void AlphaBlending();
        static void AdditiveBlending();

        static void DepthMask(bool enabled);

        static void NoDepthTesting();
        static void LessDepthTesting();
        static void EqualDepthTesting();
        static void LequalDepthTesting();
        static void GreaterDepthTesting();
        static void NotEqualDepthTesting();
        static void GequalDepthTesting();
        static void AlwaysDepthTesting();

        static void ClearColor(const glm::vec4& color);

        static void UseFramebuffers(bool enabled);

        static void NoCulling();
        static void FrontCulling();
        static void BackCulling();
        static void FrontAndBackCulling();

        static void DrawModePolygonPoints();
        static void DrawModeWireframe();
        static void DrawModeFill();
    private:
        void Summary();

        inline static Context* s_Context;

        enum class Facing {
            eCW = 0,
            eCCW
        } m_Facing;

        enum class Blending {
            eNone = 0,
            eAlpha,
            eAdditive
        } m_Blending;

        bool m_DepthMaskEnabled;

        enum class DepthTesting {
            eNone = 0,
            eLess,
            eEqual,
            eLequal,
            eGreater,
            eNotEqual,
            eGequal,
            eAlways
        } m_DepthTesting;

        glm::vec4 m_ClearColor;

        bool m_FramebuffersEnabled;

        enum class Culling {
            eNone = 0,
            eFront,
            eBack,
            eFrontAndBack
        } m_Culling;

        enum class PolygonMode {
            ePoint = 0,
            eLine,
            eFill
        } m_PolygonMode;
    };
    typedef std::shared_ptr<Context>    ContextPtr;
}

#define glCheckError() vd::gl::Context::CheckError(__FILE__, __LINE__)

#endif //VDGE_GL_CONTEXT_HPP

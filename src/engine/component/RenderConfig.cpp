#include "RenderConfig.hpp"

namespace vd::component
{
    // CCWConfig
    void CCWConfig::enable()
    {
        glFrontFace(GL_CCW);
    }

    void CCWConfig::disable()
    {
        glFrontFace(GL_CW);
    }

    // CWConfig
    void CWConfig::enable()
    {
        glFrontFace(GL_CW);
    }

    void CWConfig::disable()
    {
        glFrontFace(GL_CCW);
    }

    // SkyConfig
    void SkyConfig::enable() {
        glDepthFunc(GL_LEQUAL);
        glFrontFace(GL_CCW);
    }

    void SkyConfig::disable() {
        glDepthFunc(GL_LESS);
        glFrontFace(GL_CW);
    }
}
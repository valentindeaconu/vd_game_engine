#ifndef __RENDER_CONFIG_HPP_
#define __RENDER_CONFIG_HPP_

#include <engine/foundation/GL.hpp>
#include <memory>

namespace vd::component
{
    class RenderConfig
    {
    public:
        virtual void enable() = 0;
        virtual void disable() = 0;
    };
    typedef std::shared_ptr<RenderConfig> RenderConfigPtr;

    class CCWConfig : public RenderConfig
    {
    public:
        void enable() override;
        void disable() override;
    };
    typedef std::shared_ptr<CCWConfig> CCWConfigPtr;

    class CWConfig : public RenderConfig
    {
    public:
        void enable() override;
        void disable() override;
    };
    typedef std::shared_ptr<CWConfig> CWConfigPtr;

    class SkyConfig : public RenderConfig
    {
    public:
        void enable() override;
        void disable() override;
    };
    typedef std::shared_ptr<SkyConfig> SkyConfigPtr;
}

#endif // !__RENDER_CONFIG_HPP_
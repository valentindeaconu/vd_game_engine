#ifndef __RENDER_CONFIG_HPP_
#define __RENDER_CONFIG_HPP_

#include <engine/foundation/GL.hpp>

namespace vd
{
	namespace component
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
			void enable();
			void disable();
		};
		typedef std::shared_ptr<CCWConfig> CCWConfigPtr;

		class CWConfig : public RenderConfig
		{
		public:
			void enable();
			void disable();
		};
		typedef std::shared_ptr<CWConfig> CWConfigPtr;
	}
}

#endif // !__RENDER_CONFIG_HPP_
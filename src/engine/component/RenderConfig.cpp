#include "RenderConfig.hpp"

namespace vd
{
	namespace component
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
	}
}
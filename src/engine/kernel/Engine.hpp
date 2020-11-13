#ifndef __ENGINE_BLOCK_HPP_
#define __ENGINE_BLOCK_HPP_

#include <engine/api/gl/GL.hpp>

#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include <list>

#include <engine/datastruct/Observer.hpp>

#include <engine/misc/Types.hpp>

#include <engine/component/RenderingPass.hpp>

#include <engine/kernel/ObjectOfType.hpp>
#include <engine/window/Window.hpp>

namespace vd {
    class Engine : public datastruct::Observable {
	public:
		Engine();
		~Engine();

		void Link();
		void Init();
		void Start();

		void Add(const component::RenderingPass& renderingPass);
		void Remove(const std::string& renderingPassName);

        // Get the number of frames per second of the engine
        [[nodiscard]] int getFramesPerSecond() const;

        // Get the time necessary by a frame to be completely rendered
        [[nodiscard]] float getFrameTime() const;

		[[nodiscard]] const glm::vec4& getClipPlane() const;
		void setClipPlane(const glm::vec4& clipPlane);

	private:
		void Run();
		void Stop();

		void Update();
		void Render();
		void CleanUp();

		int m_FPS; // frames per second
		float m_FrameTimeInSeconds; // frame time (in seconds)

		float m_FrameTime;
		bool m_Running;

		glm::vec4 clipPlane;

		std::function<vd::Dimension()> m_DimensionGetter;
		vd::Predicate m_CloseRequestChecker;

		std::list<component::RenderingPass> m_RenderingPasses;

	};
	typedef std::shared_ptr<Engine>	EnginePtr;
}

#endif // !__ENGINE_BLOCK_HPP_
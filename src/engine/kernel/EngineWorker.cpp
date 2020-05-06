#include "EngineWorker.hpp"

namespace vd
{
	namespace kernel
	{
		void Observable::subscribe(const ObserverPtr& observer)
		{
			this->observers.push_back(observer);
		}

		void Observable::unsubscribe(const ObserverPtr& observer)
		{
			auto it = observers.begin();
			for (; it != observers.end(); ++it)
			{
				if (*it == observer)
				{
					break;
				}
			}

			observers.erase(it);
		}

		void Observable::broadcastInit()
		{
			for (auto it = observers.begin(); it != observers.end(); ++it)
			{
				(*it)->init();
			}
		}

		void Observable::broadcastUpdate()
		{
			for (auto it = observers.begin(); it != observers.end(); ++it)
			{
				(*it)->update();
			}
		}

		void Observable::broadcastCleanUp()
		{
			for (auto it = observers.begin(); it != observers.end(); ++it)
			{
				(*it)->cleanUp();
			}
		}

		EngineWorker::EngineWorker()
		{
		}

		EngineWorker::~EngineWorker()
		{
		}

		void EngineWorker::init()
		{
			broadcastInit();
		}

		void EngineWorker::update()
		{
			broadcastUpdate();
		}

		void EngineWorker::cleanUp()
		{
			broadcastCleanUp();
		}
	}
}
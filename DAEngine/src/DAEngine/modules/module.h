#pragma once

namespace da {

	class IModule
	{
	public:

		inline IModule() {};

		inline virtual void update() {};
		inline virtual void lateUpdate() {};
		inline virtual void initialize(){};
		inline virtual void shutdown() {};
		inline virtual void lateShutdown() {};

	};

}
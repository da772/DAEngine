#pragma once

namespace da {

	class IModule
	{
	public:

		inline IModule() {};

		inline virtual void update() {};
		inline virtual void lateUpdate() {};
		inline virtual void initalize() = 0;
		inline virtual void shutdown() = 0;

	};

}
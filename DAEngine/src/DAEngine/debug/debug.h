#pragma once

#ifdef DA_DEBUG

namespace da::debug
{
	class CDebug {
	public:
		static void initialize();
		static void update(float dt);
		static void shutdown();
	};
}
#endif
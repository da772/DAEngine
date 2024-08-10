#pragma once
#include "core.h"
#ifdef DA_REVIEW

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
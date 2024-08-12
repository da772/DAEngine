#pragma once

#include "dastd.h"

namespace da::graphics
{
	class CSkeletalAnimator;

	class CAnimationManager
	{
	public:
		static void updateBegin(float deltaTime);
		static void updateEnd();

	private:
		static std::mutex s_mutex;
		static int s_count;
		static std::unordered_map<void*, bool> s_animators;
	};
}
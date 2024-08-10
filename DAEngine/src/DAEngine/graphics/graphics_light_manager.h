#pragma once

#include "daglm.h"
#include "dastd.h"

namespace da::graphics
{
	enum class ELightType : uint8_t
	{
		Point,
		Spot
	};

	// vertex buffers seem to be aligned to 16 bytes
	struct FLightData
	{
		glm::vec3 position;
		uint32_t padding;
		// radiant intensity in W/sr
		// can be calculated from radiant flux
		glm::vec3 intensity;
		float radius;
		glm::vec3 direction;
		float angle;
	};

	class CLightManager {
	public:
		static uint32_t addLight(const FLightData& light);
		static void updateLight(uint32_t i, const glm::vec3& position);
		static void updateLight(uint32_t i, const glm::vec3& position, const glm::vec3& direction);
		static void updateLight(uint32_t i, const FLightData& data);
		static void removeLight(uint32_t id);
		static void registerOnLightEvent(const std::function<void(const FLightData& light, bool added)>& func);
		static void unregisterOnLightEvent(const std::function<void(const FLightData& light, bool added)>& func);
		static const std::vector<FLightData>& getLights();

	private:
		std::vector<FLightData> m_lights;
		std::vector<std::function<void(const FLightData& light, bool added)>> m_callbacks;
		uint32_t m_globalIndex;

		static CLightManager sm_instance;
	};
}
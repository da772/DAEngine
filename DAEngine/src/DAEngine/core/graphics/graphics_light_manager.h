#pragma once

namespace da::graphics
{
	// vertex buffers seem to be aligned to 16 bytes
	struct FPointLightData
	{
		glm::vec3 position;
		uint32_t padding;
		// radiant intensity in W/sr
		// can be calculated from radiant flux
		glm::vec3 intensity;
		float radius;
	};

	class CLightManager {
	public:
		static uint32_t addLight(const FPointLightData& light);
		static void updateLight(uint32_t i, const glm::vec3& position);
		static void removeLight(uint32_t id);
		static void registerOnLightEvent(const std::function<void(const FPointLightData& light, bool added)>& func);
		static void unregisterOnLightEvent(const std::function<void(const FPointLightData& light, bool added)>& func);
		static const std::vector<FPointLightData>& getLights();

	private:
		std::vector<FPointLightData> m_lights;
		std::vector<std::function<void(const FPointLightData& light, bool added)>> m_callbacks;
		uint32_t m_globalIndex;

		static CLightManager sm_instance;
	};
}
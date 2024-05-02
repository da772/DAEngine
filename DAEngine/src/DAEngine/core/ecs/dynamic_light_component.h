#pragma once

#include "component.h"

namespace da::core {
	class CDynamicLightComponent {
		COMPONENT_H_NO_UPDATE(CDynamicLightComponent);
	public:
		CDynamicLightComponent(CEntity& parent);
		void onInitialize();
		uint32_t addPointLight(const glm::vec3& position, const glm::vec3& intensity, float radius);
		uint32_t addSpotLight(const glm::vec3& position, const glm::vec3& intensity, const glm::vec3& direction, float radius, float falloffAngle);
		void updateLight(uint32_t id, const glm::vec3& position);
		void updateLight(uint32_t id, const glm::vec3& position, const glm::vec3& direction);
		void removeLight(uint32_t id);
		void onShutdown();
	private:
		std::vector<uint32_t> m_lights;

	};
}
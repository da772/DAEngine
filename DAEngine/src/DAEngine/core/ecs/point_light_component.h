#pragma once

#include "component.h"

namespace da::core {
	class CPointLightComponent {
		COMPONENT_H_NO_UPDATE(CPointLightComponent);
	public:
		CPointLightComponent(CEntity& parent);
		void onInitialize();
		uint32_t addLight(const glm::vec3& position, const glm::vec3& intensity, float radius);
		void updateLight(uint32_t id, const glm::vec3& position);
		void removeLight(uint32_t id);
		void onShutdown();
	private:
		std::vector<uint32_t> m_lights;

	};
}
#include "dapch.h"
#include "point_light_component.h"
#include <core/graphics/graphics_light_manager.h>

namespace da::core {

	COMPONENT_CPP_NO_UPDATE(CPointLightComponent)

	CPointLightComponent::CPointLightComponent(CEntity& parent) : m_guid(CGuid::Generate()), m_parent(parent)
	{

	}

	void CPointLightComponent::onInitialize()
	{

	}

	void CPointLightComponent::onShutdown()
	{
		for (size_t i = 0; i < m_lights.size(); i++) {
			da::graphics::CLightManager::removeLight(m_lights[i]);
		}


		m_lights = {};
	}

	void CPointLightComponent::removeLight(uint32_t id)
	{
		for (size_t i = 0; i < m_lights.size(); i++) {
			if (m_lights[i] == id) {
				da::graphics::CLightManager::removeLight(id);
				m_lights.erase(m_lights.begin() + i);
				return;
			}
		}
	}

	uint32_t CPointLightComponent::addLight(const glm::vec3& position, const glm::vec3& intensity, float radius)
	{
		da::graphics::FPointLightData lightData;
		lightData.position = position;
		lightData.intensity = intensity;
		lightData.radius = radius;
		uint32_t index = da::graphics::CLightManager::addLight(lightData);
		ASSERT(index);
		m_lights.push_back(index);
		return index;
	}

	void CPointLightComponent::updateLight(uint32_t id, const glm::vec3& position)
	{
		da::graphics::CLightManager::updateLight(id, position);
	}

}
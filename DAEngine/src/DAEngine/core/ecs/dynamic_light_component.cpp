#include "dapch.h"
#include "dynamic_light_component.h"
#include <core/graphics/graphics_light_manager.h>

namespace da::core {

	COMPONENT_CPP_NO_UPDATE(CDynamicLightComponent)

	CDynamicLightComponent::CDynamicLightComponent(CEntity& parent) : m_guid(CGuid::Generate()), m_parent(parent)
	{

	}

	void CDynamicLightComponent::onInitialize()
	{

	}

	void CDynamicLightComponent::onShutdown()
	{
		for (size_t i = 0; i < m_lights.size(); i++) {
			da::graphics::CLightManager::removeLight(m_lights[i]);
		}


		m_lights = {};
	}

	void CDynamicLightComponent::removeLight(uint32_t id)
	{
		for (size_t i = 0; i < m_lights.size(); i++) {
			if (m_lights[i] == id) {
				da::graphics::CLightManager::removeLight(id);
				m_lights.erase(m_lights.begin() + i);
				return;
			}
		}
	}


	uint32_t CDynamicLightComponent::addSpotLight(const glm::vec3& position, const glm::vec3& intensity, const glm::vec3& direction, float radius, float falloffAngle)
	{
		da::graphics::FLightData lightData;
		lightData.position = position;
		lightData.intensity = intensity;
		lightData.radius = radius;
		lightData.direction = direction;
		lightData.angle = falloffAngle;
		uint32_t index = da::graphics::CLightManager::addLight(lightData);
		ASSERT(index);
		m_lights.push_back(index);
		return index;
	}


	uint32_t CDynamicLightComponent::addPointLight(const glm::vec3& position, const glm::vec3& intensity, float radius)
	{
		da::graphics::FLightData lightData;
		lightData.position = position;
		lightData.intensity = intensity;
		lightData.radius = radius;
		lightData.direction = glm::vec3(0.f);
		lightData.angle = -1.f;
		uint32_t index = da::graphics::CLightManager::addLight(lightData);
		ASSERT(index);
		m_lights.push_back(index);
		return index;
	}

	void CDynamicLightComponent::updateLight(uint32_t id, const glm::vec3& position)
	{
		da::graphics::CLightManager::updateLight(id, position);
	}

	void CDynamicLightComponent::updateLight(uint32_t id, const glm::vec3& position, const glm::vec3& direction)
	{
		da::graphics::CLightManager::updateLight(id, position, direction);
	}

}

#include "graphics_light_manager.h"

namespace da::graphics
{
	da::graphics::CLightManager CLightManager::sm_instance;

	uint32_t CLightManager::addLight(const FLightData& light)
	{
		FLightData lightData = light;
		lightData.padding = ++sm_instance.m_globalIndex;
		sm_instance.m_lights.push_back(std::move(lightData));
		for (uint32_t i = 0; i < sm_instance.m_callbacks.size(); i++) {
			sm_instance.m_callbacks[i](sm_instance.m_lights[i], true);
		}
		return sm_instance.m_globalIndex;
	}

	void CLightManager::removeLight(uint32_t id)
	{
		for (uint32_t i = 0; i < sm_instance.m_lights.size(); i++) {
			if (sm_instance.m_lights[i].padding == id) {
				const FLightData data = sm_instance.m_lights[i];
				sm_instance.m_lights.erase(sm_instance.m_lights.begin() + i);
				for (uint32_t j = 0; j < sm_instance.m_callbacks.size(); j++) {
					sm_instance.m_callbacks[j](data, false);
				}
				return;
			}
		}
	}

	void CLightManager::registerOnLightEvent(const std::function<void(const FLightData& light, bool added)>& func)
	{
		sm_instance.m_callbacks.push_back(func);
	}

	void CLightManager::unregisterOnLightEvent(const std::function<void(const FLightData& light, bool added)>& func)
	{
		for (uint32_t i = 0; i < sm_instance.m_callbacks.size(); i++) {
			if (sm_instance.m_callbacks[i].target<void(const FLightData & light, bool added)>()  == func.target<void(const FLightData & light, bool added)>()) {
				sm_instance.m_callbacks.erase(sm_instance.m_callbacks.begin() + i);
				return;
			}
		}
	}

	const std::vector<da::graphics::FLightData>& CLightManager::getLights()
	{
		return sm_instance.m_lights;
	}

	void CLightManager::updateLight(uint32_t id, const glm::vec3& position)
	{
		for (uint32_t i = 0; i < sm_instance.m_lights.size(); i++) {
			if (sm_instance.m_lights[i].padding == id) {
				sm_instance.m_lights[i].position = position;
				for (uint32_t j = 0; j < sm_instance.m_callbacks.size(); j++) {
					sm_instance.m_callbacks[j](sm_instance.m_lights[i], false);
				}
				return;
			}
		}
	}

	void CLightManager::updateLight(uint32_t id, const FLightData& data)
	{
		for (uint32_t i = 0; i < sm_instance.m_lights.size(); i++) {
			if (sm_instance.m_lights[i].padding == id) {
				sm_instance.m_lights[i] = data;
				for (uint32_t j = 0; j < sm_instance.m_callbacks.size(); j++) {
					sm_instance.m_callbacks[j](sm_instance.m_lights[i], false);
				}
				return;
			}
		}
	}

	void CLightManager::updateLight(uint32_t id, const glm::vec3& position, const glm::vec3& direction)
	{
		for (uint32_t i = 0; i < sm_instance.m_lights.size(); i++) {
			if (sm_instance.m_lights[i].padding == id) {
				sm_instance.m_lights[i].position = position;
				sm_instance.m_lights[i].direction = direction;
				for (uint32_t j = 0; j < sm_instance.m_callbacks.size(); j++) {
					sm_instance.m_callbacks[j](sm_instance.m_lights[i], false);
				}
				return;
			}
		}
	}

}
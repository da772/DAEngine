#pragma once

#include <DAEngine/core.h>
#include <daengine/physics.h>
#include "daengine/graphics.h"

namespace da
{
	class CEntity;
}

namespace da
{
	struct FHitData;
	class IVehicle;
}

namespace da
{
	class CWindowModule;
}

class CVehicleGui : da::CGui {
public:
	inline CVehicleGui(const da::CGraphicsApi& graphics) : da::CGui(graphics) {}

	inline void renderPos(const glm::vec3& pos, float dt) { m_pos = pos; onRender(dt); }

protected:
	void onRender(float dt);

private:
	glm::vec3 m_pos;
};

class CVehicle
{
public:
	CVehicle(const da::CGraphicsModule& graphics, uint32_t id = 0);
	void initialize(da::CWindowModule* window, const da::FVehicleData& vehicleData, const glm::vec3& pos = {0.f,0.f,1.f}, bool proxy = false);
	void update(float dt);
	void shutdown();
	da::CEntity* getEntity() const;

private:
	void updateWheels(float dt);
	void onKeyboardInput(const da::CEvent& e);
	void setBrakeLights(bool on);
	void setHeadLights(bool on);
	void updateLights();
	void processInput(float dt);

private:
	da::CEntity* m_entity;
	std::vector<da::CEntity*> m_wheels;
	float m_acceleration = 0.f;
	float m_steering = 0.f;
	da::IVehicle* m_vehicle = nullptr;
	bool m_controlCamera = true;
	da::CWindowModule* m_window;

	bool m_proxy;

	float m_engineMaxPower = 6000.f;
	float m_brakeMaxPower = 300.f;
	float m_maxSteer = 0.45f;
	float m_idleBrake = 10.f;

	float m_enginePower = 0.f;
	float m_steer = 0.f;

	uint32_t m_brakeLightL = 0, m_brakeLightR = 0;
	uint32_t m_headLightL = 0, m_headLightR = 0;

	uint32_t m_id;

	CVehicleGui m_gui;

};
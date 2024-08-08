#pragma once
#include <daengine.h>
#include <daengine/core.h>
#include <daengine/components.h>
#include <daengine/script.h>
#include <daengine/graphics.h>
#include "level.h"


class CCharacter;
class CVehicle;

class CTestBed01Level : public ILevel {

public:
	CTestBed01Level(CHashString name, da::modules::CGraphicsModule& graphics, da::modules::CWindowModule& window);
	void initialize();
	void update(float dt);
	void lateUpdate(float dt) override;
	void shutdown();


private:
	void createVehicle();
	void destroyVehicle();
	void onKeyboardEvent(const da::core::events::CEvent& event);

private:
	da::graphics::CSkeletalAnimation* m_swordSlashAnimation;
	da::graphics::CSkeletalAnimation* m_runAnimation;
	da::core::CCamera* m_camera = nullptr;
	CCharacter* m_character, *m_ai;
	CVehicle* m_vehicle;
	da::script::CScriptClass m_scrlevel;
	bool m_freeCam = false;

};
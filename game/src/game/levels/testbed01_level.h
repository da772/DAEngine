#pragma once
#include <daengine/core.h>
#include <daengine/components.h>
#include <daengine/script.h>
#include <daengine/graphics.h>
#include "level.h"


class CCharacter;
class CVehicle;

class CTestBed01Level : public ILevel {

public:
	CTestBed01Level(CHashString name, da::CGraphicsModule& graphics, da::CWindowModule& window);
	void initialize();
	void update(float dt);
	void lateUpdate(float dt) override;
	void shutdown();


private:
	void createVehicle();
	void destroyVehicle();
	void onKeyboardEvent(const da::CEvent& event);

private:
	da::CSkeletalAnimation* m_swordSlashAnimation;
	da::CSkeletalAnimation* m_runAnimation;
	da::CCamera* m_camera = nullptr;
	CCharacter* m_character, *m_ai;
	CVehicle* m_vehicle;
	da::CScriptClass m_scrlevel;
	bool m_freeCam = false;

};
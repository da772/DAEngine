#pragma once
#include <daengine.h>
#include <daengine/core.h>
#include <daengine/components.h>
#include <daengine/script.h>
#include "level.h"


class CCharacter;
class CVehicle;

class CTestBed01Level : public ILevel {

public:
	CTestBed01Level(CHashString name, da::modules::CWindowModule& window);
	void initialize();
	void update(float dt);
	void shutdown();


private:
	void createVehicle();
	void destroyVehicle();

private:
	da::graphics::CSkeletalAnimation* m_swordSlashAnimation;
	da::graphics::CSkeletalAnimation* m_runAnimation;
	CCharacter* m_character;
	CVehicle* m_vehicle;
	da::modules::CWindowModule& m_window;
	da::script::CScriptClass m_scrlevel;

};
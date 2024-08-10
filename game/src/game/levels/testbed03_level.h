#pragma once
#include <daengine/core.h>
#include <daengine/components.h>
#include <daengine/script.h>
#include "level.h"
#include "procgen/level_gen.h"


class CCharacter;
class CVehicle;

class CTestBed03Level : public ILevel {

public:
	CTestBed03Level(CHashString name, da::modules::CGraphicsModule& graphics, da::modules::CWindowModule& window);
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
	CLevelGenerator m_levelGen;

};
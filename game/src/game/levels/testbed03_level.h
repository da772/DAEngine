#pragma once
#include <daengine/core.h>
#include <daengine/components.h>
#include <daengine/script.h>
#include <daengine/graphics.h>
#include "level.h"


class CCharacter;
class CVehicle;

class CTestBed03Level : public ILevel {

public:
	CTestBed03Level(CHashString name, da::modules::CGraphicsModule& graphics, da::modules::CWindowModule& window);
	void initialize();
	void update(float dt);
	void lateUpdate(float dt) override;
	void shutdown();


private:
	void onKeyboardEvent(const da::core::events::CEvent& event);

private:
	da::graphics::CSkeletalAnimation* m_swordSlashAnimation;
	da::graphics::CSkeletalAnimation* m_runAnimation;
	da::core::CCamera* m_camera = nullptr;
	CCharacter* m_character, * m_ai;
	da::script::CScriptClass m_scrlevel;
	bool m_freeCam = false;

};
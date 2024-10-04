#pragma once

#include <daengine/core.h>
#include <platform/app/app.h>
#include "levels/level_selector.h"


class CCharacter;

class CGame : public da::CApp {

public:
	inline CGame(int argc, const char** argv) : CApp(argc, argv) {
		
	}

	inline ~CGame()
	{

	}

	void createModules();
	void windowEvent(const da::CEvent& e);

protected:
	virtual void onInitialize() override;
	virtual void onShutdown() override;
#ifdef DA_REVIEW
	void resetScriptDebug(bool soft, bool* b);
#endif
	virtual void onUpdate(float dt) override;
	virtual void onLateUpdate(float dt) override;

private:
	CLevelSelector* m_levelSelector;
	da::CGraphicsModule* m_graphicsModule;
	da::CGraphicsModule* m_graphicsModule2;
	da::CWindowModule* m_window = 0;
	
	bool m_showScriptDebug = false;
	bool m_showScriptDebugHard = false;

};

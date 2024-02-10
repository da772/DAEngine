#pragma once

#include <daengine.h>

namespace da
{
	namespace modules {
		class CGraphicsModule;
		class CWindowModule;
	}

	namespace graphics
	{
		class CMaterial;
	}

	namespace core
	{
		class CEntity;
	}
}

class CGame : public da::CApp {

public:
	inline CGame(int argc, const char** argv) : CApp(argc, argv) {
		
	}

	inline ~CGame()
	{

	}

	void createModules();
	void windowEvent(const da::core::events::CEvent& e);

protected:
	virtual void onInitialize() override;
	virtual void onShutdown() override;
	void renderScriptDebug(bool soft, bool* b);
	virtual void onUpdate(float dt) override;

private:
	da::modules::CGraphicsModule* m_graphicsModule;
	da::modules::CGraphicsModule* m_graphicsModule2;
	da::graphics::CMaterial* m_boltMat = 0;
	da::graphics::CMaterial* m_cubeMat = 0;
	da::graphics::CMaterial* m_cubeMat2 = 0;
	da::modules::CWindowModule* m_window = 0;
	da::core::CEntity* e1, * e2, * e3, * e4, * e5, * e6, *e7;
	bool m_showScriptDebug = false;
	bool m_showScriptDebugHard = false;

};

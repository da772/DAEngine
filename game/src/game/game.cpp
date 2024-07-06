// engine
#include <daengine/script/script_engine.h>
#include <daengine/graphics.h>
#include <daengine/debug.h>

// game
#include "game.h"
#include "levels/testbed01_level.h"
#include "levels/testbed02_level.h"
#include "levels/testbed03_level.h"
#include "script/game_script_native.h"

void CGame::onInitialize()
{
	createModules();
	CGameScriptNative::registerNatives();
#ifdef DA_REVIEW
	da::debug::CDebugMenuBar::register_debug(HASHSTR("Script"), HASHSTR("Reload Scripts"), &m_showScriptDebug, [&] {resetScriptDebug(true, &m_showScriptDebug); });
	da::debug::CDebugMenuBar::register_debug(HASHSTR("Script"), HASHSTR("Reload Scripts (Hard)"), &m_showScriptDebugHard, [&] {resetScriptDebug(false, &m_showScriptDebugHard); });
#endif

	m_levelSelector = new CLevelSelector(*m_window);
	m_levelSelector->addLevel(new CTestBed01Level(HASHSTR("TestBed01"), *m_window));
	m_levelSelector->addLevel(new CTestBed02Level(HASHSTR("TestBed02"), *m_window));
	m_levelSelector->addLevel(new CTestBed03Level(HASHSTR("TestBed03"), *m_window));
}

void CGame::onUpdate(float dt)
{
	
	m_levelSelector->update(dt);

}

void CGame::createModules()
{
	m_window = new da::modules::CWindowModule({ "game", 1280, 720, 0,0, 144, da::core::EWindowDeco::NONE });
	addModule(m_window);

	m_window->getEventHandler().registerCallback(EEventCategory::Window, BIND_EVENT_FN(CGame, windowEvent));

	m_graphicsModule = new da::modules::CGraphicsModule(m_window);
	addModule(m_graphicsModule);

	da::modules::CImGuiModule* imGuiModule = new da::modules::CImGuiModule(m_graphicsModule);
	addModule(imGuiModule);

	da::core::CCamera::setCamera(new da::core::CCamera());
}

void CGame::windowEvent(const da::core::events::CEvent& e)
{

	if (e.getType() == EEventType::WindowClose) {
		da::CLogger::LogDebug(da::ELogChannel::Application, "WindowClosed!");
		forceEnd();
		return;
	}
}

void CGame::onShutdown()
{
#ifdef DA_REVIEW
	da::debug::CDebugMenuBar::unregister_debug(HASHSTR("Script"), HASHSTR("Reload Scripts"));
	da::debug::CDebugMenuBar::unregister_debug(HASHSTR("Script"), HASHSTR("Reload Scripts (Hard)"));
	da::debug::CDebugMenuBar::unregister_debug(HASHSTR("Level"), HASHSTR("End"));
#endif
	da::CLogger::LogDebug(da::ELogChannel::Application, "App End");
	m_levelSelector->shutdown();
	delete m_levelSelector;
}

#ifdef DA_REVIEW
void CGame::resetScriptDebug(bool soft, bool* b)
{
	auto& components = da::core::CSceneManager::getScene()->getComponents<da::core::CScriptComponent>();
	da::script::CScriptEngine::clearAll();
	for (size_t i = 0; i < components.getCount(); i++) {
		da::core::CScriptComponent* c = (da::core::CScriptComponent*)components.getComponentAtIndex(i);
		c->reload(soft);
	}
	*b = false;
}
#endif

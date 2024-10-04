// engine
#include <script/script_engine.h>
#include <daengine/graphics.h>
#include <daengine/debug.h>

// game
#include "game.h"
#include "components/game_components.h"
#include "levels/testbed01_level.h"
#include "levels/testbed02_level.h"
#include "levels/testbed03_level.h"
#include "script/game_script_native.h"

void CGame::onInitialize()
{
	PROFILE()
	createModules();
	CGameScriptNative::registerNatives();
	CGameComponents::registerComponents();
#ifdef DA_REVIEW
	da::CDebugMenuBar::register_debug(HASHSTR("Script"), HASHSTR("Reload Scripts"), &m_showScriptDebug, [&] {resetScriptDebug(true, &m_showScriptDebug); });
	da::CDebugMenuBar::register_debug(HASHSTR("Script"), HASHSTR("Reload Scripts (Hard)"), &m_showScriptDebugHard, [&] {resetScriptDebug(false, &m_showScriptDebugHard); });
#endif

	m_levelSelector = new CLevelSelector(*m_window);
	m_levelSelector->addLevel(new CTestBed01Level(HASHSTR("TestBed01"), *m_graphicsModule, *m_window));
	m_levelSelector->addLevel(new CTestBed02Level(HASHSTR("TestBed02"), *m_graphicsModule, *m_window));
	m_levelSelector->addLevel(new CTestBed03Level(HASHSTR("TestBed03"), *m_graphicsModule, *m_window));

#ifndef DA_REVIEW
	m_levelSelector->startLevel(2);
#endif
}

void CGame::onUpdate(float dt)
{
	PROFILE()
	m_levelSelector->update(dt);
}

void CGame::onLateUpdate(float dt)
{
	PROFILE()
	m_levelSelector->lateUpdate(dt);
}

void CGame::createModules()
{
	m_window = new da::CWindowModule({ "game", 1280, 720, 0,0, 144, da::EWindowDeco::NONE });
	addModule(m_window);

	m_window->getEventHandler().registerCallback(EEventCategory::Window, BIND_EVENT_FN(CGame, windowEvent));

	m_graphicsModule = new da::CGraphicsModule(m_window);
	addModule(m_graphicsModule);

	da::CImGuiModule* imGuiModule = new da::CImGuiModule(m_graphicsModule);
	addModule(imGuiModule);

	da::CCamera::setCamera(new da::CCamera());
}

void CGame::windowEvent(const da::CEvent& e)
{

	if (e.getType() == EEventType::WindowClose) {
		da::CLogger::LogDebug(da::ELogChannel::Application, "WindowClosed!");
		forceEnd();
		return;
	}
}

void CGame::onShutdown()
{
	PROFILE()
#ifdef DA_REVIEW
	da::CDebugMenuBar::unregister_debug(HASHSTR("Script"), HASHSTR("Reload Scripts"));
	da::CDebugMenuBar::unregister_debug(HASHSTR("Script"), HASHSTR("Reload Scripts (Hard)"));
	da::CDebugMenuBar::unregister_debug(HASHSTR("Level"), HASHSTR("End"));
#endif
	da::CLogger::LogDebug(da::ELogChannel::Application, "App End");
	m_levelSelector->shutdown();
	delete m_levelSelector;
}

#ifdef DA_REVIEW
void CGame::resetScriptDebug(bool soft, bool* b)
{
	auto& components = da::CSceneManager::getScene()->getComponents<da::CScriptComponent>();
	da::CScriptEngine::clearAll();
	for (size_t i = 0; i < components.getCount(); i++) {
		da::CScriptComponent* c = (da::CScriptComponent*)components.getComponentAtIndex(i);
		c->reload(soft);
	}
	*b = false;
}
#endif

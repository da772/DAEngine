
#include "app.h"
#include "core.h"
#include "core/logger.h"
#include "core/arg_handler.h"
#include "ecs/components.h"
#include "core/ecs/scene.h"
#include "core/factory.h"
#include "script/script_engine.h"
#include "core/ecs/scene_manager.h"
#include "core/threading/worker_pool.h"
#include "stl/time.h"
#include "ai/navmesh/nav_mesh_manager.h"
#include "net/network_manager.h"
#include "platform/app/handlers/crash_handler.h"

#include "physics/physics.h"

#ifdef DA_REVIEW
#include "debug/debug.h"
#include "debug/debug_menu_bar.h"
#endif
#include <platform/app/animation_manager.h>

namespace da
{

	CApp::CApp(int argc, const char** argv) : m_running(true), m_modules() {
		initalizeInternal(argc, argv);
	}

	CApp::~CApp() {
		shutdownInternal();
#ifdef DA_REVIEW
		if (m_captureProfile) PROFILE_END_CAPTURE()
#endif
	}

	void CApp::initialize()
	{
		m_initialized = true;
#ifndef DA_TEST
		CScriptEngine::initialize();
#ifdef DA_REVIEW
		da::CDebug::initialize();
		da::CDebugMenuBar::register_debug(HASHSTR("App"), HASHSTR("Reset"), &m_reset, [&] { reset();});
		da::CDebugMenuBar::register_debug(HASHSTR("ECS"), HASHSTR("CSceneManager"), &m_sceneDebug, [&] { renderSceneDebug();  });
		da::CDebugMenuBar::register_debug(HASHSTR("ECS"), HASHSTR("ECSDebug"), &m_ecsDebug, [&] { renderECSDebug();  });
#endif
#endif

		da::CNetworkManager::initialize();
		CSceneManager::initialize();
		da::CPhysics::initialize();
		da::CNavMeshManager::initialize();
		for (IModule* m : m_modules) {
			m->initialize();
		}
		onInitialize();
		if (CScene* scene = CSceneManager::getScene()) {
			scene->initialize();
		}
		
	}
	void CApp::update()
	{
		while (m_running)
		{
			PROFILE_FRAME("Main")
			CCrashHandler::registerCrashHandler();
			double timeStep = da::CTime::newFrame();
			{
				PROFILE("AnimManager Update Begin")
				da::CAnimationManager::updateBegin(timeStep);
			}
			{
				PROFILE("Module Update")
				for (IModule* m : m_modules) {
					m->update();
				}
			}
#ifdef DA_REVIEW
			{
				PROFILE("Debug Update")
				da::CDebug::update(timeStep);
			}
			{
				PROFILE("Script Debug Update")
				da::CScriptEngine::update();
			}
#endif
			{
				PROFILE("App Update")
				onUpdate(timeStep);
			}

			if (CScene* scene = CSceneManager::getScene()) {
				PROFILE("Scene Update")
				scene->update(timeStep);
			}

			{
				PROFILE("WorkerPool Update")
				da::CWorkerPool::update();
			}

			{
				PROFILE("NavMesh Update")
				da::CNavMeshManager::update(timeStep);
			}

			{
				PROFILE("Physics Update")
				da::CPhysics::update(timeStep);
			}
			
			{
				PROFILE("AnimManager Update End")
				da::CAnimationManager::updateEnd();
			}
			
			{
				PROFILE("App Late Update")
				onLateUpdate(timeStep);
			}

			{
				PROFILE("Module Late Update")
				for (IModule* m : m_modules) {
					m->lateUpdate();
				}
			}
			
			if (m_reset) {
				shutdown();
				initialize();
				m_reset = false;
			}
		}
 	}
	void CApp::shutdown()
	{
		onShutdown();

		if (CScene* scene = CSceneManager::getScene()) {
			scene->shutdown();
			delete scene;
			CSceneManager::setScene(nullptr);
		}

		for (IModule* m : m_modules) {
			m->shutdown();
		}

		for (IModule* m : m_modules) {
			m->lateShutdown();
			delete m;
		}
		m_modules = {};
		da::CNavMeshManager::shutdown();
		da::CPhysics::shutdown();
		CSceneManager::shutdown();
		da::CNetworkManager::shutdown();
#ifndef DA_TEST
		CScriptEngine::shutdown();
#ifdef DA_REVIEW
		da::CDebugMenuBar::unregister_debug(HASHSTR("App"), HASHSTR("Reset"));
		da::CDebugMenuBar::unregister_debug(HASHSTR("ECS"), HASHSTR("CSceneManager"));
		da::CDebugMenuBar::unregister_debug(HASHSTR("ECS"), HASHSTR("ECSDebug"));
		da::CDebug::shutdown();
#endif
#endif
	}

	void CApp::reset()
	{
		m_reset = true;
	}

	void CApp::addModule(IModule* module)
	{
		m_modules.push_back(module);
		if (m_initialized) module->initialize();
	}

	void CApp::forceEnd()
	{
		m_running = false;
	}

	void CApp::initalizeInternal(int argc, const char** argv)
	{
		CArgHandler::initialize(argc, argv);
#ifdef DA_REVIEW
		m_captureProfile = CArgHandler::contains(HASHSTR("profileCapture"));
		if (m_captureProfile) PROFILE_START_CAPTURE()
#endif
		CLogger::initialize();
		CWorkerPool::initialize();
		std::string args = "Initialized with argc: %d\n";
		for (size_t i = 0; i < argc; i++) {
			args += std::string(argv[i]);
			if (i != argc - 1)
				args += "\n";
		}
		LOG_INFO(ELogChannel::Core, args, argc);
		CComponents::registerComponents();
	}

	void CApp::shutdownInternal()
	{
#ifdef DA_REVIEW
		CFactoryDebug::checkInstances();
#endif
		CArgHandler::shutdown();
		CLogger::shutdown();
		CWorkerPool::shutdown();
#ifdef DA_REVIEW
		if (m_captureProfile) PROFILE_START_CAPTURE()
#endif
	}

#ifdef DA_REVIEW
	void CApp::renderSceneDebug()
	{
		using namespace da;
		if (CScene* scene = CSceneManager::getScene()) {

			if (ImGui::Begin("Scene Debug", &m_sceneDebug)) {

				for (CEntity* e : scene->getEntities()) {
					char nameBuffer[1024];
					sprintf_s(nameBuffer, sizeof(nameBuffer), "%s (%s)", e->getTag().c_str(), e->getId().c_str());
					if (ImGui::CollapsingHeader(nameBuffer)) {
						ImGui::Indent();

						{
							glm::vec3 p = e->getTransform().position();
							float pos[] = { p.x, p.y, p.z };
							ImGui::Text("Position: ");
							ImGui::SameLine();
							if (ImGui::InputFloat3((std::string("##Position") + std::string(e->getId().c_str())).c_str(), pos, "%.3f")) {
								p = { pos[0], pos[1], pos[2] };
								e->getTransform().setPosition(p);
							}
						}

						{
							glm::vec3 r = e->getTransform().rotationEuler();
							float rot[] = { r.x ,r.y, r.z };
							ImGui::Text("Rotation");
							ImGui::SameLine();
							if (ImGui::InputFloat3((std::string("##Rotation") + std::string(e->getId().c_str())).c_str(), rot, "%.3f")) {
								r = { rot[0], rot[1], rot[2] };
								e->getTransform().setRotation(r);
							}
						}


						{
							glm::vec3 s = e->getTransform().scale();
							float scale[] = { s.x ,s.y, s.z };
							ImGui::Text("Scale");
							ImGui::SameLine();
							if (ImGui::InputFloat3((std::string("##scale") + std::string(e->getId().c_str())).c_str(), scale, "%.3f")) {
								s = { scale[0], scale[1], scale[2] };
								e->getTransform().setScale(s);
							}
						}

						e->debugRender();
						ImGui::Unindent();
					}

				}
			}

			ImGui::End();

		}
	}

	void CApp::renderECSDebug()
	{
		using namespace da;
		if (CScene* scene = CSceneManager::getScene()) {

			if (ImGui::Begin("ECS Debug", &m_ecsDebug)) {

				for (const std::pair<CHashString, FComponentContainer>& kv : scene->getComponentContainers())
				{
					constexpr int colSize = 5;
					if (ImGui::CollapsingHeader(kv.first.c_str()))
					{
						if (kv.second.getCount())
						{
							ImGui::BeginTable(kv.first.c_str(), colSize, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg);
							for (size_t i = 0; i < kv.second.getCount() * kv.second.getComponentSize(); i += 2) {
								if (i % colSize == 0) {
									ImGui::TableNextRow();
								}
								ImGui::TableSetColumnIndex(i % colSize);
								if (i % kv.second.getComponentSize() == 0) {
									ImU32 cell_bg_color = ImGui::GetColorU32(ImVec4(0.3f, 0.3f, 0.7f, 0.65f));
									ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);
								}
								ImGui::Text("0x%04x", *((uint16_t*)kv.second.getComponentAtIndexUnscaled(i)));
							}

							ImGui::EndTable();
						}
					}
				}

			}

			ImGui::End();
		}
	}

#endif

}
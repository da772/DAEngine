#include "testbed03_level.h"

//engine
#include <daengine/ai.h>
#include <daengine/graphics.h>
#include <daengine/core.h>
#include <daengine/physics.h>

// game
#include "game/character/character.h"
#include "game/helpers/texture_helper.h"


CTestBed03Level::CTestBed03Level(CHashString name, da::modules::CGraphicsModule& graphics, da::modules::CWindowModule& window) : ILevel(name, graphics, window), m_scrlevel("scripts/build/levels/test_bed_01.lua", "TestBed01", "main", false)
{

}

void CTestBed03Level::initialize()
{
	m_windowModule.getWindow()->lockCursor(true);
	m_scrlevel.setup({}, {});
	da::core::CSceneManager::setScene(new da::core::CScene(da::core::CGuid("2f94546a-a92a-4046-946a-f28c2364110b")));
	da::core::CCamera::getCamera()->setPosition({ 0,0,1 });

	// Test Bed
	{
		da::core::CEntity* testBed = da::core::CSceneManager::getScene()->createEntity();
		da::graphics::CStaticMesh* colMesh = da::factory::CStaticMeshFactory::create("assets/plane100.fbx");
		da::core::FComponentRef<da::core::CSmeshComponent> meshComponent = testBed->addComponent<da::core::CSmeshComponent>("assets/plane.fbx");

		meshComponent->getStaticMesh()->getMaterial(0).setBaseColorTexture(CTextureHelper::create(Textures::Tex_Fine_Road_D));
		meshComponent->getStaticMesh()->getMaterial(0).metallicFactor = 0.0;
		meshComponent->getStaticMesh()->getMaterial(0).uvScale = { 100.f,100.f };
		meshComponent->getStaticMesh()->getMaterial(0).roughnessFactor = 1.0;
		meshComponent->getStaticMesh()->getMaterial(0).doubleSided = true;

		meshComponent->getStaticMesh()->castShadows(true);
		testBed->getTransform().setScale({ 100.f, 100.f, 1.f});
		testBed->addComponent<da::core::CRigidBodyComponent>(
			da::physics::IPhysicsRigidBody::create(
				da::physics::CPhysicsShapeCube::create({ 100.f, 100.f, .001f })
				, da::physics::CPhysicsDefaultMotionState::create(testBed->getTransform().matrix())
				, 0.f
				, { 0.f,0.f,0.f }));
		testBed->setTag(HASHSTR("TestBed"));

		// Nav mesh
		da::ai::CNavMeshManager::addNavMesh(*new da::ai::CTiledNavMesh(colMesh));
	}

	// Character
	m_character = new CCharacter(*m_graphicsModule.getGraphicsApi(), true);
	m_character->initialize();
	m_scrlevel.classInitialize();

	m_windowModule.getWindow()->getEventHandler().registerCallback(EEventType::InputKeyboard, BIND_EVENT_FN(CTestBed03Level, onKeyboardEvent));
}

void CTestBed03Level::onKeyboardEvent(const da::core::events::CEvent& event)
{
	const da::core::events::CInputKeyboardEvent& kb = (da::core::events::CInputKeyboardEvent&)event;

	if (kb.getBtn() != 290 || kb.getType() != EInputType::RELEASED) // F1
	{
		return;
	}

	if (!m_freeCam)
	{
		da::core::CInput::pushInputContext(HASHSTR("FreeCam"), 1e9);
	}
	else
	{
		da::core::CInput::popInputContext(HASHSTR("FreeCam"));
	}
	m_freeCam = !m_freeCam;
	m_windowModule.getWindow()->lockCursor(!m_freeCam);
}

void CTestBed03Level::update(float dt)
{
	// R
	if (da::core::CInput::inputPressed(82)) {
		auto& components = da::core::CSceneManager::getScene()->getComponents<da::core::CScriptComponent>();
		da::script::CScriptEngine::clearAll();
		for (size_t i = 0; i < components.getCount(); i++) {
			da::core::CScriptComponent* c = (da::core::CScriptComponent*)components.getComponentAtIndex(i);
			c->reload(false);
		}
		m_scrlevel.classShutdown();
		m_scrlevel.cleanup();
		m_scrlevel.setup({}, {});
		m_scrlevel.classInitialize();
	}

	m_character->update(dt);

	m_scrlevel.classUpdate(dt);
}

void CTestBed03Level::lateUpdate(float dt)
{
	m_character->lateUpdate(dt);

	if (m_freeCam)
	{
		static glm::vec2 cursorPos = { 0.f, 0.f };
		const float camSpeed = 5.f;

		if (!m_camera)
		{
			m_camera = new da::core::CCamera();
		}


		da::core::CCamera* cam = m_camera;

		if (da::core::CCamera* oldCam = da::core::CCamera::getCamera())
		{
			if (oldCam != cam)
			{
				da::core::CCamera::setCamera(cam);
				cam->setPosition(oldCam->position());
				cam->setRotation(oldCam->rotation());
			}
		}

		

		if (da::core::CInput::inputPressed(87)) // W
		{
			cam->offsetPosition(cam->forward() * camSpeed * dt);
		}

		if (da::core::CInput::inputPressed(83)) // S
		{
			cam->offsetPosition(-cam->forward() * camSpeed * dt);
		}

		if (da::core::CInput::inputPressed(65)) // A
		{
			cam->offsetPosition(-cam->right() * camSpeed * dt);
		}

		if (da::core::CInput::inputPressed(68)) // D
		{
			cam->offsetPosition(cam->right() * camSpeed * dt);
		}

		if (da::core::CInput::inputPressed(69)) // E
		{
			cam->offsetPosition(-cam->up() * camSpeed * dt);
		}

		if (da::core::CInput::inputPressed(81)) // Q
		{
			cam->offsetPosition(cam->up() * camSpeed * dt);
		}

		glm::vec2 pos = { da::core::CInput::getCursorX(), da::core::CInput::getCursorY() };
		if (da::core::CInput::mouseInputPressed(1))
		{
			if (cursorPos.x >= 0.0 && cursorPos.y >= 0.0)
			{
				cam->offsetRotation({ cursorPos.y - pos.y, 0.f, cursorPos.x - pos.x });
			}
		}
		cursorPos = pos;
	}

}

void CTestBed03Level::shutdown()
{

	m_windowModule.getWindow()->getEventHandler().unregisterCallback(EEventType::InputKeyboard, BIND_EVENT_FN(CTestBed03Level, onKeyboardEvent));

	if (m_freeCam)
	{
		da::core::CInput::popInputContext(HASHSTR("FreeCam"));
		m_windowModule.getWindow()->lockCursor(false);
		m_freeCam = false;
	}

	m_scrlevel.classShutdown();
	m_scrlevel.cleanup();

	m_ai->shutdown();
	delete m_ai;

	m_character->shutdown();
	delete m_character;

	delete m_camera;

	da::core::CSceneManager::setScene(nullptr);
}
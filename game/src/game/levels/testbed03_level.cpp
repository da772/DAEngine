#include "testbed03_level.h"

//engine
#include <daengine/ai.h>
#include <daengine/graphics.h>
#include <daengine/core.h>
#include <daengine/physics.h>

// game
#include "game/character/character.h"
#include "game/helpers/texture_helper.h"

CTestBed03Level::CTestBed03Level(CHashString name, da::CGraphicsModule& graphics, da::CWindowModule& window) : ILevel(name, graphics, window), m_scrlevel("scripts/build/levels/test_bed_01.lua", "TestBed01", "main", false)
{

}

void CTestBed03Level::initialize()
{
	m_windowModule.getWindow()->lockCursor(true);
	m_scrlevel.setup({}, {});
	da::CSceneManager::setScene(new da::CScene(da::CGuid("2f94546a-a92a-4046-946a-f28c2364110b")));
	da::CCamera::getCamera()->setPosition({ 0,0,1 });
	da::CTiledNavMesh* navMesh = new da::CTiledNavMesh();

	// Test Bed
	{
		da::CEntity* testBed = da::CSceneManager::getScene()->createEntity();
		da::CStaticMesh* colMesh = da::CStaticMeshFactory::create("assets/plane100.fbx");
		da::FComponentRef<da::CSmeshComponent> meshComponent = testBed->addComponent<da::CSmeshComponent>(colMesh);

		meshComponent->getStaticMesh()->getMaterial(0).setBaseColorTexture(da::CTexture2DFactory::Create("assets/source/textures/tex_debug_grid_01.png"));
		meshComponent->getStaticMesh()->getMaterial(0).metallicFactor = 0.0;
		meshComponent->getStaticMesh()->getMaterial(0).uvScale = { 40.f,40.f };
		meshComponent->getStaticMesh()->getMaterial(0).roughnessFactor = 1.0;
		meshComponent->getStaticMesh()->getMaterial(0).doubleSided = true;

		meshComponent->getStaticMesh()->castShadows(true);
		
		testBed->addComponent<da::CRigidBodyComponent>(
			da::IPhysicsRigidBody::create(
				da::CPhysicsShapeCube::create({ 100.f, 100.f, .001f })
				, da::CPhysicsDefaultMotionState::create(testBed->getTransform().matrix())
				, 0.f
				, { 0.f,0.f,0.f }));
		testBed->setTag(HASHSTR("TestBed"));
		
		// Nav mesh
		navMesh->addMesh(glm::mat4(1.f), colMesh->getMeshes()[0]);
	}

	// Ramp
	{
		da::CEntity* testBed = da::CSceneManager::getScene()->createEntity();
		da::CStaticMesh* colMesh = da::CStaticMeshFactory::create("assets/ramp.fbx");
		da::FComponentRef<da::CSmeshComponent> meshComponent = testBed->addComponent<da::CSmeshComponent>(colMesh);

		meshComponent->getStaticMesh()->getMaterial(0).setBaseColorTexture(da::CTexture2DFactory::Create("assets/source/textures/tex_debug_grid_01.png"));
		meshComponent->getStaticMesh()->getMaterial(0).metallicFactor = 0.0;
		meshComponent->getStaticMesh()->getMaterial(0).roughnessFactor = 1.0;
		meshComponent->getStaticMesh()->getMaterial(0).doubleSided = true;
		meshComponent->getStaticMesh()->castShadows(true);
		testBed->getTransform().setPosition({ 0.f, 0.f, 0.f });
		testBed->getTransform().setScale({ 1.0f, 1.f, 1.f });
		testBed->getTransform().setRotation({ 0.f, 0.f,0.f });
		testBed->addComponent<da::CRigidBodyComponent>(
			da::IPhysicsRigidBody::create(
				da::CPhysicsShapeTriangleMesh::create(colMesh, 0)
				, da::CPhysicsDefaultMotionState::create(testBed->getTransform().matrix())
				, 0.f
				, { 0.f,0.f,0.f }));
		testBed->setTag(HASHSTR("TestBed"));

		// Nav mesh
		navMesh->addMesh(testBed->getTransform().matrix(), colMesh->getMeshes()[0]);
	}

	// Character
	m_character = new CCharacter(*m_graphicsModule.getGraphicsApi(), true);
	m_character->initialize();

	// ai
	m_ai = new CCharacter(*m_graphicsModule.getGraphicsApi(), false);
	m_ai->initialize();

	m_scrlevel.classInitialize();


	da::CNavMeshManager::addNavMesh(*navMesh);
	m_windowModule.getWindow()->getEventHandler().registerCallback(EEventType::InputKeyboard, BIND_EVENT_FN(CTestBed03Level, onKeyboardEvent));
}

void CTestBed03Level::onKeyboardEvent(const da::CEvent& event)
{
	const da::CInputKeyboardEvent& kb = (da::CInputKeyboardEvent&)event;

	if (kb.getBtn() != 290 || kb.getType() != EInputType::RELEASED) // F1
	{
		return;
	}

	if (!m_freeCam)
	{
		da::CInput::pushInputContext(HASHSTR("FreeCam"), 1e9);
	}
	else
	{
		da::CInput::popInputContext(HASHSTR("FreeCam"));
	}
	m_freeCam = !m_freeCam;
	m_windowModule.getWindow()->lockCursor(!m_freeCam);
}

void CTestBed03Level::update(float dt)
{

	// R
	if (da::CInput::inputPressed(82)) {
		auto& components = da::CSceneManager::getScene()->getComponents<da::CScriptComponent>();
		da::CScriptEngine::clearAll();
		for (size_t i = 0; i < components.getCount(); i++) {
			da::CScriptComponent* c = (da::CScriptComponent*)components.getComponentAtIndex(i);
			c->reload(false);
		}
		m_scrlevel.classShutdown();
		m_scrlevel.cleanup();
		m_scrlevel.setup({}, {});
		m_scrlevel.classInitialize();
	}

	m_character->update(dt);
	m_ai->update(dt);

	m_scrlevel.classUpdate(dt);
}

void CTestBed03Level::lateUpdate(float dt)
{
	m_character->lateUpdate(dt);
	m_ai->lateUpdate(dt);

	if (m_freeCam)
	{
		static glm::vec2 cursorPos = { 0.f, 0.f };
		float camSpeed = 5.f;

		if (!m_camera)
		{
			m_camera = new da::CCamera();
		}


		da::CCamera* cam = m_camera;

		if (da::CCamera* oldCam = da::CCamera::getCamera())
		{
			if (oldCam != cam)
			{
				da::CCamera::setCamera(cam);
				cam->setPosition(oldCam->position());
				cam->setRotation(oldCam->rotation());
			}
		}

		if (da::CInput::inputPressed(340)) // LSHIFT
		{
			camSpeed *= 10.f;
		}

		if (da::CInput::inputPressed(341)) // LCTRL
		{
			camSpeed /= 2.f;
		}

		if (da::CInput::inputPressed(87)) // W
		{
			cam->offsetPosition(cam->forward() * camSpeed * dt);
		}

		if (da::CInput::inputPressed(83)) // S
		{
			cam->offsetPosition(-cam->forward() * camSpeed * dt);
		}

		if (da::CInput::inputPressed(65)) // A
		{
			cam->offsetPosition(-cam->right() * camSpeed * dt);
		}

		if (da::CInput::inputPressed(68)) // D
		{
			cam->offsetPosition(cam->right() * camSpeed * dt);
		}

		if (da::CInput::inputPressed(69)) // E
		{
			cam->offsetPosition(-cam->up() * camSpeed * dt);
		}

		if (da::CInput::inputPressed(81)) // Q
		{
			cam->offsetPosition(cam->up() * camSpeed * dt);
		}

		glm::vec2 pos = { da::CInput::getCursorX(), da::CInput::getCursorY() };
		if (da::CInput::mouseInputPressed(1))
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
		da::CInput::popInputContext(HASHSTR("FreeCam"));
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

	da::CSceneManager::setScene(nullptr);
}
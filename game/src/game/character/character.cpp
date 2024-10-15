#include "character.h"
#include "../components/character_movement_component.h"
#include "../components/character_component.h"

#include <DAEngine/components.h>
#include <game/components/health_component.h>
#include "game/helpers/asset_helper.h"

CCharacter::CCharacter(const da::CGraphicsApi& graphics, bool isLocalPlayer) : m_gui(graphics, this), m_isLocalPlayer(isLocalPlayer)
{

}

void CCharacter::initialize()
{
	if (da::CScene* scene = da::CSceneManager::getScene()) {
		m_entity = scene->createEntity();
		if (m_isLocalPlayer) {
			m_entity->setTag(HASHSTR("Character"));
		}
		else {
			m_entity->setTag(HASHSTR("AiCharacter"));
		}

		m_entity->addComponent<CCharacterMovementComponent>(da::CGuid::Generate());
		m_entity->addComponent<CHealthComponent>(da::CGuid::Generate());
		da::FComponentRef<CCharacterComponent> characterRef = m_entity->addComponent<CCharacterComponent>(m_isLocalPlayer, da::CGuid::Generate());

		m_sword = scene->createEntity();
		m_sword->setTag(HASHSTR("Sword"));
		m_sword->addComponent<da::CSmeshComponent>(CAssetHelper::create(Model::prop_weap_sword));
		CAssetHelper::set_material(Material::prop_weap_sword_Mat, m_sword->getComponent<da::CSmeshComponent>()->getStaticMesh());
		m_sword->getComponent<da::CSmeshComponent>()->getStaticMesh()->getMaterial(0).metallicFactor = .5f;
		m_sword->getComponent<da::CSmeshComponent>()->getStaticMesh()->getMaterial(0).roughnessFactor = .4f;
		da::IPhysicsShape* shape = da::CPhysicsShapeCube::create({ .017f, .057f, .598f });
		da::FComponentRef<da::CCollisionComponent> colComp =
			m_sword->addComponent<da::CCollisionComponent>(*shape, glm::translate(glm::mat4(1.f), { 0.f, 0.f, 1.f }), da::CGuid::Generate());

		colComp->setCallback([colComp, this](const da::FCollisionEventData& data) {
			if (!data.Overlapping) return;
			LOG_DEBUG(da::ELogChannel::Application, "Collision Event: %s, Overlapping: %d", m_entity->getTag().c_str(), data.Overlapping);
			if (da::CEntity* other = (da::CEntity*)data.Other) {
				da::FComponentRef<CHealthComponent> healthComp = other->getComponent<CHealthComponent>();
				if (healthComp.isValid()) {
					healthComp->damage(1.f);
				}

			}

			});



		characterRef->setWeaponEntity(m_sword);
	}
}

void CCharacter::update(float dt)
{

}

void CCharacter::lateUpdate(float dt)
{

	if (m_isLocalPlayer)
	{
		processCamera(dt);
	}

	m_gui.render(dt);

	da::FComponentRef<da::CSkeletalMeshComponent> skele = m_entity->getComponent<da::CSkeletalMeshComponent>();
	glm::mat4 transform;
	glm::vec3 position;
	glm::quat rot;

	if (!skele->getSkeletalAnimator()->getCurrentAnim()) return;

	if (!skele->getBoneWorldPosition(HASHSTR("hand_r"), position)) {
		return;
	}

	if (!skele->getBoneWorldRotation(HASHSTR("hand_r"), rot)) {
		return;
	}

	m_sword->getTransform().setPosition(position + rot * glm::vec3(-0.075f, -0.02f, 0.03f));
	m_sword->getTransform().setRotation(rot * glm::quat(glm::radians(glm::vec3(0.f, 0.f, 90.f))));
}

void CCharacter::shutdown()
{
	da::CSceneManager::getScene()->removeEntity(m_entity);
	da::CSceneManager::getScene()->removeEntity(m_sword);
	delete m_camera;
}

const da::CEntity* CCharacter::getEntity() const
{
	return m_entity;
}

void CCharacter::processCamera(float dt)
{
	if (!m_camera)
	{
		m_camera = new da::CCamera();
	}

	da::CInputContext inputCtx(HASHSTR("CharacterInput"), 0);

	if (da::CInput::inputContextAvailable())
	{
		da::CCamera::setCamera(m_camera);
	}

	da::CCamera* cam = m_camera;
	const float xPos = da::CInput::getCursorX();
	const float yPos = da::CInput::getCursorY();

	da::FComponentRef<CCharacterComponent> character = m_entity->getComponent<CCharacterComponent>();

	if (true/*da::CInput::mouseInputPressed(1)*/)
	{
		float yDiff = yPos - m_cursorPos.y;
		m_camHeight += glm::radians(yDiff) * m_camSensitivity;
		m_camHeight = std::clamp(m_camHeight, -1.5f, 3.f);
	}

	m_camRot = character->getCamRot();

	glm::vec3 xOff = (m_camDist * std::cos(m_camRot)) * glm::vec3(1.f, 0.f, 0.f);
	glm::vec3 yOff = (m_camDist * std::sin(m_camRot)) * glm::vec3(0.f, 1.f, 0.f);
	double yScroll = da::CInput::getScrollY();

	if (m_scrollY != yScroll) {
		m_camDist -= 5.f * dt * yScroll;
		m_camDist = std::clamp(m_camDist, 1.f, 5.f);
		m_scrollY = yScroll;
	}

	m_cursorPos = { xPos, yPos };

	glm::vec3 pos = m_entity->getTransform().position() + glm::vec3(0.f, 0.f, m_camHeight) + xOff + yOff;
	
	if (cam->position() != pos)
	{
		PROFILE("Camera Sweep")
		glm::vec3 endPos = pos;

		static da::IPhysicsShape* shape = nullptr;
		const float probeSize = .2f;

		if (!shape)
		{
			shape = da::CPhysicsShapeCube::create({ probeSize, probeSize ,probeSize });
		}

		glm::mat4 posTransform = glm::translate(glm::mat4(1.f), pos);

		da::FSweepData sweep(shape, m_entity->getTransform().matrix(), posTransform);
		da::CPhysics::sweepTest(sweep);

		//da::CDebugRender::drawLine(m_entity->getTransform().position(), endPos, .01f, { 1.f,0.f,0.f,1.f });
		if (sweep.bHit)
		{
			//LOG_DEBUG(da::ELogChannel::Application, "hit: %f, %f, %f", sweep.Hit.position.x, sweep.Hit.position.y, sweep.Hit.position.z);
			pos = sweep.Hit.position + (sweep.Hit.normal * (probeSize/2.f));
			//da::CDebugRender::drawLine(m_entity->getTransform().position(), pos, .01f, { 0.f,1.f,0.f,1.f });
			//da::CDebugRender::drawCube(sweep.Hit.position, glm::quat(), { .2f,.2f,.2f }, { 0.f,1.f,0.f, 1.f });
			pos = glm::mix(cam->position(), pos, .05f);
		}

		
	}



	if (!da::CInput::inputContextAvailable())
	{
		return;
	}

	cam->setPosition(pos);
	cam->lookAt(m_entity->getTransform().position());
}


void CCharacterGui::onRender(float dt)
{
#ifdef DA_REVIEW
	return;
	glm::vec3 headPos;
	da::FComponentRef<da::CSkeletalMeshComponent> skele = m_parent->m_entity->getComponent<da::CSkeletalMeshComponent>();
	if (!skele->getSkeletalAnimator()->getCurrentAnim()) return;

	if (!skele->getBoneWorldPosition(HASHSTR("mixamorig:Head"), headPos)) {
		return;
	}

	glm::vec3 worldSpace = m_parent->m_entity->getTransform().position() + glm::vec3(0.f, 0.f, 1.5f);

	glm::vec2 pos = worldPosToScreenSpace(headPos + glm::vec3(0.f, 0.f, .25f));
	float dist = std::abs(glm::distance(headPos, da::CCamera::getCamera()->position()));

	if (dist < 1.0f) {
		dist = 1.0f;
	}

	glm::vec2 size = { 120.f, 40.f };

	ImGui::SetNextWindowSize({ size.x, size.y });
	ImGui::SetNextWindowPos({ pos.x - size.x / 2.f, pos.y - size.y / 2.f });
	ImGui::PushStyleColor(ImGuiCol_FrameBg, { 0.f,0.f,0.f,0.f });
	ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.f,0.f,0.f,0.f });
	ImGui::PushStyleColor(ImGuiCol_Border, { 0.f,0.f,0.f,0.f });
	ImGui::PushStyleColor(ImGuiCol_PlotHistogram, { 0.f,1.f,0.f,1.f });
	if (ImGui::Begin((std::string("###hp") + std::string(m_parent->m_entity->getId().c_str())).c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
	{
		da::FComponentRef<CHealthComponent> healthComp = m_parent->m_entity->getComponent<CHealthComponent>();
		//auto windowWidth = ImGui::GetWindowSize().x;
		//auto textWidth = ImGui::CalcTextSize("00 Health").x;
		//
		//ImGui::SetCursorPosX((size.x - textWidth) * 0.5f);
		//ImGui::LabelText("###hp", "%2.f Health", healthComp->getHealth());

		ImGui::ProgressBar(healthComp->getHealth() / 100.f);
	}
	ImGui::PopStyleColor(4);

	ImGui::End();
#endif
}




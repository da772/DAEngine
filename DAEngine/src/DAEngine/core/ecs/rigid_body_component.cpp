#include "dapch.h"
#include "rigid_body_component.h"
#include <core/events/event_handler.h>
#include "DAEngine/physics/physics_rigid_body.h"
#include <core/graphics/graphics_debug_render.h>
#include "skeletal_mesh_component.h"
#include "core/graphics/skeletal_animator.h"

#ifdef DA_REVIEW
#include <Imgui.h>
#endif

namespace da::core {
#ifdef DA_REVIEW
	COMPONENT_CPP_NO_UPDATE_DEBUG(CRigidBodyComponent)
#else
	COMPONENT_CPP_NO_UPDATE(CRigidBodyComponent)
#endif

	CRigidBodyComponent::CRigidBodyComponent(da::physics::IPhysicsRigidBody* rigidBody, CEntity& parent) : m_guid(CGuid::Generate()), m_parent(parent), m_rigidBody(rigidBody)
	{

	}

	void CRigidBodyComponent::onInitialize()
	{
		m_rigidBody->setUserData((void*)&m_parent);
		m_rigidBody->reset();
	}

	void CRigidBodyComponent::onShutdown()
	{

	}

	da::physics::IPhysicsRigidBody* CRigidBodyComponent::getPhysicsBody() const
	{
		return m_rigidBody;
	}

#ifdef DA_REVIEW
	void CRigidBodyComponent::onDebugRender()
	{
		ImGui::Text("Debug Render: ");
		ImGui::SameLine();
		ImGui::Checkbox("##debugRender", &m_debugRender);

		if (m_debugRender)
		{
			m_rigidBody->debugDraw();
		}

	}
#endif

	CRigidBodyComponent::~CRigidBodyComponent()
	{
		ASSERT(m_rigidBody);
		delete m_rigidBody;
	}

}
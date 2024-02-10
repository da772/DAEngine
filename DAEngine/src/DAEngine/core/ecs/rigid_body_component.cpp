#include "dapch.h"
#include "rigid_body_component.h"
#include <core/events/event_handler.h>
#include "DAEngine/physics/physics_rigid_body.h"
#include <core/graphics/graphics_debug_render.h>

namespace da::core {
#ifdef DA_DEBUG
	COMPONENT_CPP_NO_UPDATE_DEBUG(CRigidBodyComponent)
#else
	COMPONENT_CPP_NO_UPDATE(CRigidBodyComponent)
#endif

	CRigidBodyComponent::CRigidBodyComponent(da::physics::IPhysicsRigidBody* rigidBody, CEntity& parent) : m_guid(CGuid::Generate()), m_parent(parent), m_rigidBody(rigidBody)
	{

	}

	void CRigidBodyComponent::onInitialize()
	{
		m_rigidBody->setUserData((void*)& m_parent.getTransform());
	}

	void CRigidBodyComponent::onShutdown()
	{

	}

	da::physics::IPhysicsRigidBody* CRigidBodyComponent::getPhysicsBody() const
	{
		return m_rigidBody;
	}

#ifdef DA_DEBUG
	void CRigidBodyComponent::onDebugRender()
	{
		ImGui::Text("Debug Render: ");
		ImGui::SameLine();
		ImGui::Checkbox("##debugRender", &m_debugRender);

		if (m_debugRender)
		{
			switch (m_rigidBody->getShape()->getType()) {
				case da::physics::EPhysicsShapeType::Cube: 
				{
					da::graphics::CDebugRender::getInstance()->drawCube(m_parent.getTransform().position(), m_parent.getTransform().rotation(), m_parent.getTransform().scale(), {1.f,0.f,0.f,1.f}, true, false);
					break;
				}
				case da::physics::EPhysicsShapeType::Sphere:
				{
					da::graphics::CDebugRender::getInstance()->drawSphere(m_parent.getTransform().position(), m_parent.getTransform().rotation(), m_parent.getTransform().scale(), { 1.f,0.f,0.f,1.f }, true, false);
					break;
				}
				case da::physics::EPhysicsShapeType::TriangleMesh:
				{
					if (da::physics::CPhysicsShapeTriangleMesh* shape = dynamic_cast<da::physics::CPhysicsShapeTriangleMesh*>(m_rigidBody->getShape()))
					{
						da::graphics::CDebugRender::getInstance()->drawMesh(m_parent.getTransform().position(), m_parent.getTransform().rotation(), m_parent.getTransform().scale(), shape->getMesh(), { 1.f,0.f,0.f,1.f }, true, false);
					}
					break;
				}
			}
		}

	}
#endif
}
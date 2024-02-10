#pragma once

#include "component.h"

namespace da::physics {
	class IPhysicsRigidBody;
}

namespace da::core {

	class CRigidBodyComponent
	{
#ifdef DA_DEBUG
		COMPONENT_H_NO_UPDATE_DEBUG(CRigidBodyComponent)
#else
		COMPONENT_H_NO_UPDATE(CRigidBodyComponent)
#endif

	public:
		CRigidBodyComponent(da::physics::IPhysicsRigidBody* rigidBody, CEntity& parent);

		void onInitialize();
		void onShutdown();
		da::physics::IPhysicsRigidBody* getPhysicsBody() const;
#ifdef DA_DEBUG
		void onDebugRender();
#endif

	private:
#ifdef DA_DEBUG
		bool m_debugRender = true;
#endif

	private:
		da::physics::IPhysicsRigidBody* m_rigidBody;


	};
}
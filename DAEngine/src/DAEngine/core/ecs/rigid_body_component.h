#pragma once

#include "component.h"

namespace da::physics {
	class IPhysicsRigidBody;
}

namespace da::core {

	class CRigidBodyComponent
	{
#ifdef DA_REVIEW
		COMPONENT_H_NO_UPDATE_DEBUG(CRigidBodyComponent)
#else
		COMPONENT_H_NO_UPDATE(CRigidBodyComponent)
#endif

	public:
		CRigidBodyComponent(da::physics::IPhysicsRigidBody* rigidBody, CEntity& parent);
		~CRigidBodyComponent();

		void onInitialize();
		void onShutdown();
		da::physics::IPhysicsRigidBody* getPhysicsBody() const;
#ifdef DA_REVIEW
		void onDebugRender();
#endif

	private:
#ifdef DA_REVIEW
		bool m_debugRender = true;
#endif

	private:
		da::physics::IPhysicsRigidBody* m_rigidBody;


	};
}
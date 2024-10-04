#pragma once

#include "core/ecs/component.h"

namespace da {
	class IPhysicsRigidBody;
}

namespace da {

	class CRigidBodyComponent
	{
#ifdef DA_REVIEW
		COMPONENT_H_NO_UPDATE_DEBUG(CRigidBodyComponent)
#else
		COMPONENT_H_NO_UPDATE(CRigidBodyComponent)
#endif

	public:
		CRigidBodyComponent(da::IPhysicsRigidBody* rigidBody, CEntity& parent);
		~CRigidBodyComponent();

		void onInitialize();
		void onShutdown();
		da::IPhysicsRigidBody* getPhysicsBody() const;
#ifdef DA_REVIEW
		void onDebugRender();
#endif

	private:
#ifdef DA_REVIEW
		bool m_debugRender = true;
#endif

	private:
		da::IPhysicsRigidBody* m_rigidBody;


	};
}
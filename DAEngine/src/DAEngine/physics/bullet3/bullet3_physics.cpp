#include "dapch.h"
#include "bullet3_physics.h"

#include "daengine/core/graphics/graphics_debug_render.h"
#include <glm/gtx/matrix_decompose.hpp>
#include <imgui.h>
#include <core/graphics/camera.h>

namespace da::physics
{
	
	void CBullet3Physics::initialize()
	{
		m_collisionConfiguration = new btDefaultCollisionConfiguration();
		m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
		m_overlappingPairCache = new btDbvtBroadphase();
		m_solver = new btSequentialImpulseConstraintSolver();

		m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_overlappingPairCache, m_solver, m_collisionConfiguration);

		m_dynamicsWorld->setGravity(btVector3(0, 0, -10));
	}

	void CBullet3Physics::update(float dt)
	{
		for (int j = m_dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
		{
			btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[j];
			btRigidBody* body = btRigidBody::upcast(obj);
			
			if (body && body->getMotionState())
			{
				if (da::maths::CTransform* e = (da::maths::CTransform*)body->getUserPointer()) {
					if (e->isDirty()) {
						btTransform trans;
						body->setActivationState(ACTIVE_TAG);
						trans.setFromOpenGLMatrix(glm::value_ptr(e->matrix()));
						body->setWorldTransform(trans);
					}
				}
			}

		}

		m_dynamicsWorld->stepSimulation(dt, 10, 1.f / 60.f);

		//print positions of all objects
		for (int j = m_dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
		{
			btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[j];
			btRigidBody* body = btRigidBody::upcast(obj);

			if (body && body->getMotionState())
			{
				btTransform trans;
				body->getMotionState()->getWorldTransform(trans);
				glm::mat4 transform;
				trans.getOpenGLMatrix(glm::value_ptr(transform));

				if (da::maths::CTransform* e = (da::maths::CTransform*)body->getUserPointer()) {
					e->setTransform(transform);
				}
			}	
		}
	}

	void CBullet3Physics::shutdown()
	{
		for (int i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
		{
			btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
			btRigidBody* body = btRigidBody::upcast(obj);
			if (body && body->getMotionState())
			{
				delete body->getMotionState();
			}
			m_dynamicsWorld->removeCollisionObject(obj);
			delete obj;
		}

		//delete dynamics world
		delete m_dynamicsWorld;

		//delete solver
		delete m_solver;

		//delete broadphase
		delete m_overlappingPairCache;

		//delete dispatcher
		delete m_dispatcher;

		delete m_collisionConfiguration;
	}

}
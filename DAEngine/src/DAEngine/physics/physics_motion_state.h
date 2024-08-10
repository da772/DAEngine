#pragma once

#include "daglm.h"

namespace da::core
{
	class CEntity;
}

namespace da::physics
{
	class IPhysicsMotionState
	{
	protected:
		inline IPhysicsMotionState() {};
		inline virtual ~IPhysicsMotionState() {};
	};

	class CPhysicsDefaultMotionState : public IPhysicsMotionState
	{
	public:
		static CPhysicsDefaultMotionState* create(const glm::mat4& transform);
	protected:
		inline CPhysicsDefaultMotionState(const glm::mat4& transform) {};
		inline virtual ~CPhysicsDefaultMotionState() {};


	};

	class CPhysicsEntityMotionState : public IPhysicsMotionState
	{
	public:
		static CPhysicsEntityMotionState* create(da::core::CEntity* entity);
	protected:
		inline CPhysicsEntityMotionState(da::core::CEntity* entity) : m_entity(entity){};
		inline virtual ~CPhysicsEntityMotionState() {};
		da::core::CEntity* m_entity = nullptr;


	};
}
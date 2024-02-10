#pragma once

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
}
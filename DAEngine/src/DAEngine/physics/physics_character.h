#pragma once

namespace da::physics
{
	class IPhysicsRigidBody;

	struct FCharacterTuning
	{

	};

	class ICharacterController
	{
	public:
		inline ICharacterController(const FCharacterTuning& data, IPhysicsRigidBody& rigidBody) {};
		inline virtual ~ICharacterController() {};
	};

	class CharacterControllerFactory
	{
	public:
		static ICharacterController* Create(const FCharacterTuning& data, IPhysicsRigidBody& rigidBody);
	};
}
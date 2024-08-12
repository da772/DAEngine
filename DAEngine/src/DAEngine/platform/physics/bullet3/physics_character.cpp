
#include "physics/physics_character.h"
#include "bullet3_character.h"
#include "physics/physics_shape.h"

namespace da::physics
{

	da::physics::ICharacterController* CharacterControllerFactory::create(const FCharacterTuning& data, IPhysicsShape& shape)
	{
		return new CBullet3Character(data, shape);
	}

}
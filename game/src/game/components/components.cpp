#include "components.h"
#include "character_movement_component.h"
#include "character_component.h"
#include "health_component.h"

void CComponents::registerComponents()
{
	CCharacterComponent::registerComponent();
	CCharacterMovementComponent::registerComponent();
	CHealthComponent::registerComponent();

}

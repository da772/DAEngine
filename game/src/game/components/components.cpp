#include "components.h"
#include "character_movement_component.h"
#include "character_component.h"

void CComponents::registerComponents()
{
	CCharacterMovementComponent::registerComponent();
	CCharacterComponent::registerComponent();
}

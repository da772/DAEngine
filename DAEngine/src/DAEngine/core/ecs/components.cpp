#include "dapch.h"
#include "components.h"

namespace da::core {

	void CComponents::registerComponents()
	{
		CTestComponent::registerComponent();
		CScriptComponent::registerComponent();
		CSmeshComponent::registerComponent();
		CSkeletalMeshComponent::registerComponent();
		CRigidBodyComponent::registerComponent();
		CDynamicLightComponent::registerComponent();
		CNetworkedTransformComponent::registerComponent();
	}

}
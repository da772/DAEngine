
#include "components.h"

namespace da {

	void CComponents::registerComponents()
	{
		CTestComponent::registerComponent();
		CScriptComponent::registerComponent();
		CSmeshComponent::registerComponent();
		CSkeletalMeshComponent::registerComponent();
		CRigidBodyComponent::registerComponent();
		CDynamicLightComponent::registerComponent();
		CCollisionComponent::registerComponent();


		CNetworkedTransformComponent::registerComponent();
	}

}
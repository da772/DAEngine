#include "dapch.h"
#include "components.h"

#include "test_component.h"
#include "script_component.h"
#include "smesh_component.h"

namespace da::core {

	void CComponents::registerComponents()
	{
		CTestComponent::registerComponent();
		CScriptComponent::registerComponent();
		CSmeshComponent::registerComponent();
	}

}

#include "test_component.h"
#include "core/ecs/scene.h"
#include <core/events/event_handler.h>

namespace da::core {
	
	COMPONENT_CPP(CTestComponent);

	CTestComponent::CTestComponent(const char* data1, const char* data2, CEntity& parent) 
		: m_guid(CGuid::Generate()), data1(data1), data2(data2), m_parent(parent)
	{
		
		
	}

	void CTestComponent::onInitialize()
	{
		LOG_DEBUG(ELogChannel::Core, "Initializing with data %s, %s", data1.c_str(), data2.c_str());
	}

	void CTestComponent::onUpdate(float dt)
	{
		//LOG_DEBUG(ELogChannel::Core, "Update with data %s, %s", data1.c_str(), data2.c_str());
	}

	void CTestComponent::onShutdown()
	{
		LOG_DEBUG(ELogChannel::Core, "Shutdown with data %s, %s", data1.c_str(), data2.c_str());
	}

}
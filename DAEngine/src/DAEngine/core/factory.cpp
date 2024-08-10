
#include "factory.h"


namespace da::core
{
#ifdef DA_REVIEW
	void CFactoryDebug::checkInstances()
	{
		/*
		#define CHECK_FACTORY(x) if (x::ms_factory.hasInstances()) { LOG_ERROR(ELogChannel::Core, "Factory has instances: %s, Count: %d", #x, x::ms_factory.m_instances.size()); printInstances(x::ms_factory.getInstanceNames()); }

		CHECK_FACTORY(da::graphics::CStaticMeshFactory);
		CHECK_FACTORY(da::graphics::CTexture2DFactory);
		CHECK_FACTORY(da::graphics::CMaterialFactory);
		CHECK_FACTORY(da::graphics::CSkeletalMeshFactory);
		*/

	}

	void CFactoryDebug::printInstances(const std::vector<std::pair<CHashString, uint32_t>>& instances)
	{
		for (uint32_t i = 0; i < instances.size(); i++) {
			LOG_DEBUG(ELogChannel::Core, "%s : %d", instances[i].first.c_str(), instances[i].second);
		}
		
	}

#endif

}
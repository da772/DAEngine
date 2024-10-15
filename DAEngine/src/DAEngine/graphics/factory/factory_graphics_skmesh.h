#pragma once
#include "core/factory.h"
#include "graphics/graphics_skmesh.h"

using namespace da;

namespace da {

	class CSkeletalMeshFactory : da::CFactory<CSkeletalMesh>
	{
	public:
		static CSkeletalMesh* create(const std::string& path, bool binary = false, bool inverseNormals = false);
		static void remove(CSkeletalMesh* mesh);

	private:
		static CSkeletalMeshFactory ms_factory;

#ifdef DA_REVIEW
		friend class da::CFactoryDebug;
#endif
	};
}
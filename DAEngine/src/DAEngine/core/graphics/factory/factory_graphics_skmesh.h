#pragma once
#include "DAEngine/core/factory.h"
#include "DAEngine/core/graphics/graphics_skmesh.h"

namespace da::graphics {

	class CSkeletalMeshFactory : da::core::CFactory<CSkeletalMesh>
	{
	public:
		static CSkeletalMesh* create(const std::string& path, bool inverseNormals = false);
		static void remove(CSkeletalMesh* mesh);

	private:
		static CSkeletalMeshFactory ms_factory;

#ifdef DA_REVIEW
		friend class da::core::CFactoryDebug;
#endif
	};
}
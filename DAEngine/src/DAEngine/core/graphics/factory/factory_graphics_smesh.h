#pragma once

#include "DAEngine/core/factory.h"
#include "DAEngine/core/graphics/graphics_smesh.h"

namespace da::graphics
{
	class CStaticMeshFactory : da::core::CFactory<CStaticMesh>
	{
		public:
			static CStaticMesh* create(const std::string& path, bool inverseNormals = false);
			static void remove(CStaticMesh* mesh);

		private:
			static CStaticMeshFactory ms_factory;
#ifdef DA_REVIEW
			friend class da::core::CFactoryDebug;
#endif

	};
}
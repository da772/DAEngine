#pragma once

#include "core/factory.h"
#include "graphics/graphics_smesh.h"

using namespace da::graphics;

namespace da::factory
{
	extern class CStaticMeshFactory : da::core::CFactory<CStaticMesh>
	{
		public:
			static CStaticMesh* create(const std::string& path, bool process = true);
			static void remove(CStaticMesh* mesh);

		private:
			static CStaticMeshFactory ms_factory;
#ifdef DA_REVIEW
			friend class da::core::CFactoryDebug;
#endif

	};
}
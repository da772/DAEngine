#include "dapch.h"
#include "factory_graphics_smesh.h"
#include "DAEngine/platform/graphics/bgfx/bgfx_static_mesh.h"

namespace da::graphics
{
	da::graphics::CStaticMeshFactory CStaticMeshFactory::ms_factory;

	da::graphics::CStaticMesh* CStaticMeshFactory::create(const std::string& path, bool inverseNormals)
	{
		CHashString hash(path.c_str(), path.size());
#ifdef DA_GRAPHICS_BGFX
		return ms_factory.addInstance(hash, [path, inverseNormals]() { return new da::platform::CBgfxStaticMesh(path, inverseNormals); });
#endif
	}

	void CStaticMeshFactory::remove(CStaticMesh* mesh)
	{
		CHashString hash(mesh->getPath().c_str(), mesh->getPath().size());
		ms_factory.removeInstance(hash);
	}
}
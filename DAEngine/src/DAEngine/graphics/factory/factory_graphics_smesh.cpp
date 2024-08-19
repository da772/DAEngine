
#include "graphics/factory/factory_graphics_smesh.h"
#include "platform/graphics/bgfx/bgfx_static_mesh.h"

namespace da::factory
{
	CStaticMeshFactory CStaticMeshFactory::ms_factory;

	da::graphics::CStaticMesh* CStaticMeshFactory::create(const std::string& path, bool process)
	{
		CHashString hash(path.c_str(), path.size());
#ifdef DA_GRAPHICS_BGFX
		return ms_factory.addInstance(hash, [&path, process]() { return new da::platform::CBgfxStaticMesh(path, process); });
#endif
	}

	void CStaticMeshFactory::remove(CStaticMesh* mesh)
	{
		CHashString hash(mesh->getPath().c_str(), mesh->getPath().size());
		ms_factory.removeInstance(hash);
	}
}
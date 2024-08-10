
#include "core/core.h"
#include "graphics/factory/factory_graphics_skmesh.h"

#ifdef DA_GRAPHICS_BGFX
#include "platform/graphics/bgfx/bgfx_skeletal_mesh.h"
#endif

namespace da::factory
{

	CSkeletalMeshFactory da::factory::CSkeletalMeshFactory::ms_factory;

	da::graphics::CSkeletalMesh* da::factory::CSkeletalMeshFactory::create(const std::string& path, bool inverseNormals /*= false*/)
	{
		CHashString hash(path.c_str(), path.size());
#ifdef DA_GRAPHICS_BGFX
		return ms_factory.addInstance(hash, [path, inverseNormals] { return new da::platform::CBgfxSkeletalMesh(path, inverseNormals); });
#endif
	}

	void da::factory::CSkeletalMeshFactory::remove(CSkeletalMesh* mesh)
	{
		CHashString hash(mesh->getPath().c_str(), mesh->getPath().size());
		ms_factory.removeInstance(hash);
	}

}
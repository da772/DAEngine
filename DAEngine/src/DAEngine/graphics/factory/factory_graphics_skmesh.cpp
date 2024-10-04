
#include "core.h"
#include "graphics/factory/factory_graphics_skmesh.h"

#ifdef DA_GRAPHICS_BGFX
#include "platform/graphics/bgfx/bgfx_skeletal_mesh.h"
#endif

namespace da
{

	CSkeletalMeshFactory da::CSkeletalMeshFactory::ms_factory;

	da::CSkeletalMesh* da::CSkeletalMeshFactory::create(const std::string& path, bool inverseNormals /*= false*/)
	{
		CHashString hash(path.c_str(), path.size());
#ifdef DA_GRAPHICS_BGFX
		return ms_factory.addInstance(hash, [path, inverseNormals] { return new da::CBgfxSkeletalMesh(path, inverseNormals); });
#endif
	}

	void da::CSkeletalMeshFactory::remove(CSkeletalMesh* mesh)
	{
		CHashString hash(mesh->getPath().c_str(), mesh->getPath().size());
		ms_factory.removeInstance(hash);
	}

}
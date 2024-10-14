#include "asset_helper.h"
#include "dacommon.h"
#include "game/generated/file_map_generator.h"


da::CStaticMesh* CAssetHelper::create(Model model)
{
	return da::CStaticMeshFactory::create(CFileMapGenerator::get_filepath(CHashString((uint64_t)model)), false);
}

da::CGraphicsTexture2D* CAssetHelper::create(Texture texture)
{
	return da::CTexture2DFactory::Create(CFileMapGenerator::get_filepath(CHashString((uint64_t)texture)));
}

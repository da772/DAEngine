#include "model_helper.h"
#include "dacommon.h"


da::graphics::CStaticMesh* CModelHelper::create(Model model)
{
	da::core::CGuid guid = da::core::CGuid::Generate((uint32_t)model);
	const char path[] = "assets/Models/7b280000-6062-0000-7207-0000277e0000.mdel";
	memcpy((void*)&path[14], guid.c_str(), sizeof(da::core::guid_str) - 1);
	return da::factory::CStaticMeshFactory::create(path, false);
}

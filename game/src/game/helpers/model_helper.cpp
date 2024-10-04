#include "model_helper.h"
#include "dacommon.h"


da::CStaticMesh* CModelHelper::create(Model model)
{
	da::CGuid guid = da::CGuid::Generate((uint32_t)model);
	const char path[] = "assets/Models/7b280000-6062-0000-7207-0000277e0000.mdel";
	memcpy((void*)&path[14], guid.c_str(), sizeof(da::guid_str) - 1);
	return da::CStaticMeshFactory::create(path, false);
}

#include "texture_helper.h"
#include "dacommon.h"

da::CGraphicsTexture2D* CTextureHelper::create(Texture texture)
{
	da::CGuid guid = da::CGuid::Generate((uint32_t)texture);
	const char path[] = "assets/Textures/7b280000-6062-0000-7207-0000277e0000.dds";
	memcpy((void*)&path[16], guid.c_str(), sizeof(da::guid_str)-1);
	return da::CTexture2DFactory::Create(path);
}

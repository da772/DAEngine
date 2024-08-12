#include "dastd.h"
#include "graphics/factory/factory_graphics_material.h"
#include "platform/graphics/bgfx/bgfx_graphics_material.h"
#include "debug/debug_menu_bar.h"

da::factory::CMaterialFactory da::factory::CMaterialFactory::ms_factory;

da::graphics::CMaterial* da::factory::CMaterialFactory::create(const std::string& vsPath, const std::string& fsPath)
{
	CHashString vsHash(vsPath.c_str()), fsHash(fsPath.c_str());
	CHashString combinedHash(vsHash.hash(), fsHash.hash());

#ifdef DA_REVIEW
	//LOG_INFO(ELogChannel::Graphics, "Creating shader (%zu): %s, %s", combinedHash.hash(), vsPath.c_str(), fsPath.c_str());
#endif

#ifdef DA_GRAPHICS_BGFX
	CMaterial* mat = ms_factory.addInstance(combinedHash, [vsPath, fsPath, combinedHash] { return new da::platform::CBgfxGraphicsMaterial(vsPath, fsPath, combinedHash); });
#ifdef DA_REVIEW
	if (ms_factory.m_instances.size() == 1) {
		da::debug::CDebugMenuBar::register_debug(HASHSTR("Shaders"), HASHSTR("Reload Shaders"), &ms_factory.m_debug, reloadShaders);
	}
#endif

	return mat;
#endif
}

da::graphics::CMaterial* da::factory::CMaterialFactory::create(const std::string& csPath)
{
	CHashString csHash(csPath.c_str());
#ifdef DA_GRAPHICS_BGFX
	CMaterial* mat = ms_factory.addInstance(csHash, [csPath, csHash] {return new da::platform::CBgfxGraphicsMaterial(csPath, csHash); });
	return mat;
#endif
}

void da::factory::CMaterialFactory::remove(CMaterial* material)
{

	ms_factory.removeInstance(material->getHash());

#ifdef DA_REVIEW
	if (ms_factory.m_instances.empty()) {
		da::debug::CDebugMenuBar::unregister_debug(HASHSTR("Shaders"), HASHSTR("Reload"));
	}
#endif
}

#ifdef DA_REVIEW
void da::factory::CMaterialFactory::reloadShaders()
{
	for (const std::pair<CHashString, FInstanceInfo>& kv : ms_factory.m_instances) {
		kv.second.Ptr->shutdown();
	}

	for (const std::pair<CHashString, FInstanceInfo>& kv : ms_factory.m_instances) {
		kv.second.Ptr->initialize();
	}

	ms_factory.m_debug = false;
}
#endif
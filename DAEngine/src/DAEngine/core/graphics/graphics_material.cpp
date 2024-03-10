#include "dapch.h"
#include "DAEngine/core/graphics/graphics_material.h"
#include "DAEngine/core/graphics/factory/factory_graphics_texture2d.h"

#ifdef DA_GRAPHICS_BGFX
#include "DAEngine/platform/graphics/bgfx/bgfx_graphics_material.h"
#endif

#ifdef DA_REVIEW
#include "DAEngine/debug/debug_menu_bar.h"
#include <Imgui.h>
#endif


namespace da::graphics
{
	std::unordered_map<CHashString, FMaterialInfo> CMaterialFactory::ms_materials;

#ifdef DA_REVIEW
	bool CMaterialFactory::m_debug;
#endif


	da::graphics::CMaterial* CMaterialFactory::create(const std::string& vsPath, const std::string& fsPath)
	{
		CHashString vsHash(vsPath.c_str()), fsHash(fsPath.c_str());
		CHashString combinedHash(vsHash.hash(), fsHash.hash());

		const std::unordered_map<CHashString, FMaterialInfo>::iterator& it = ms_materials.find(combinedHash);

		if (it != ms_materials.end()) {
			ASSERT(it->second.Count);
			it->second.Count++;
			return it->second.Material;
		}

#ifdef DA_GRAPHICS_BGFX
		da::graphics::CMaterial* mat = new da::platform::CBgfxGraphicsMaterial(vsPath, fsPath);
		mat->m_hash = combinedHash;
		ms_materials[combinedHash] = { mat, 1 };

#ifdef DA_REVIEW
		if (ms_materials.size() == 1) {
			da::debug::CDebugMenuBar::register_debug(HASHSTR("Shaders"), HASHSTR("Reload Shaders"), &m_debug, reloadShaders);
		}
#endif

		return mat;
#endif
	}

	da::graphics::CMaterial* CMaterialFactory::create(const std::string& csPath)
	{
		CHashString csHash(csPath.c_str());

		const std::unordered_map<CHashString, FMaterialInfo>::iterator& it = ms_materials.find(csHash);

		if (it != ms_materials.end()) {
			ASSERT(it->second.Count);
			it->second.Count++;
			return it->second.Material;
		}

#ifdef DA_GRAPHICS_BGFX
		da::graphics::CMaterial* mat = new da::platform::CBgfxGraphicsMaterial(csPath);
		mat->m_hash = csHash;
		ms_materials[csHash] = { mat, 1 };
		return mat;
#endif
	}

	void CMaterialFactory::remove(CMaterial* material)
	{
		const std::unordered_map<CHashString, FMaterialInfo>::iterator& it = ms_materials.find(material->getHash());

		if (it == ms_materials.end()) {
			return;
		}

		ASSERT(it->second.Count);
		if (--it->second.Count) {
			return;
		}

		delete it->second.Material;

		ms_materials.erase(it);

#ifdef DA_REVIEW
		if (ms_materials.empty()) {
			da::debug::CDebugMenuBar::unregister_debug(HASHSTR("Shaders"), HASHSTR("Reload"));
		}
#endif
	}

#ifdef DA_REVIEW
	void CMaterialFactory::reloadShaders()
	{
		for (const std::pair<CHashString, FMaterialInfo>& kv : ms_materials) {
			kv.second.Material->shutdown();
		}

		for (const std::pair<CHashString, FMaterialInfo>& kv : ms_materials) {
			kv.second.Material->initialize();
		}

		m_debug = false;
	}
#endif
}
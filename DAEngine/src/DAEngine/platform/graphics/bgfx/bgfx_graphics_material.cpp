
#include "bgfx_graphics_material.h"

#ifdef DA_GRAPHICS_BGFX
#include <bgfx/bgfx.h>
#include "bgfx_util.h"

#include "core/asset/asset.h"
#include "platform/graphics/bgfx/bgfx_graphics_api.h"

namespace da {

	std::unordered_map<CHashString, FShaderData> CBgfxGraphicsMaterial::ms_shaderHandles;

	CBgfxGraphicsMaterial::CBgfxGraphicsMaterial(const std::string& vsPath, const std::string& fsPath, CHashString hash) : m_vsPath(vsPath), m_fsPath(fsPath)
	{
		m_hash = hash;
		getPlatformPath(m_vsPath);
		getPlatformPath(m_fsPath);

		initialize();
	}

	CBgfxGraphicsMaterial::CBgfxGraphicsMaterial(const std::string& csPath, CHashString hash) : m_csPath(csPath)
	{
		m_hash = hash;
		getPlatformPath(m_csPath);

		initialize();
	}

	CBgfxGraphicsMaterial::CBgfxGraphicsMaterial()
	{

	}

	void CBgfxGraphicsMaterial::initialize()
	{
		if (!m_csPath.empty()) {

			CHashString csHash(m_csPath.c_str());
			const std::unordered_map<CHashString, FShaderData>::iterator& it = ms_shaderHandles.find(csHash);

			::bgfx::ShaderHandle handle1 = BGFX_INVALID_HANDLE;
			if (it != ms_shaderHandles.end()) {
				it->second.Count++;
				handle1 = { it->second.Handle };
			}
			else {
				handle1 = { createShader(m_csPath, csHash) };
			}

			m_program = ::bgfx::createProgram(handle1, false).idx;
			return;
		}

		::bgfx::ShaderHandle handle1 = BGFX_INVALID_HANDLE;
		::bgfx::ShaderHandle handle2 = BGFX_INVALID_HANDLE;

		CHashString vsHash(m_vsPath.c_str()), fsHash(m_fsPath.c_str());

		{
			const std::unordered_map<CHashString, FShaderData>::iterator& it = ms_shaderHandles.find(vsHash);

			if (it != ms_shaderHandles.end()) {
				it->second.Count++;
				handle1 = { it->second.Handle };
			}
			else {
				handle1 = { createShader(m_vsPath, vsHash) };
			}

			ASSERT(::bgfx::isValid(handle1));
		}

		{
			const std::unordered_map<CHashString, FShaderData>::iterator& it = ms_shaderHandles.find(fsHash);

			if (it != ms_shaderHandles.end()) {
				it->second.Count++;
				handle2 = { it->second.Handle };
			}
			else {
				handle2 = { createShader(m_fsPath, fsHash) };
			}

			ASSERT(::bgfx::isValid(handle2));
		}

		m_program = ::bgfx::createProgram(handle1, handle2, false).idx;
	}


	uint16_t CBgfxGraphicsMaterial::createShader(const std::string& path, CHashString hash)
	{
		CAsset vs(path);
		const ::bgfx::Memory* mem = ::bgfx::copy(vs.data(), (uint32_t)(vs.size() + 1ull));

		::bgfx::ShaderHandle handle = ::bgfx::createShader(mem);
		::bgfx::setName(handle, path.c_str());

		ms_shaderHandles[hash] = { handle.idx, 1 };
		return handle.idx;
	}

	void CBgfxGraphicsMaterial::shutdown()
	{
		{
			CHashString vsHash(m_vsPath.c_str());
			const std::unordered_map<CHashString, FShaderData>::iterator& it = ms_shaderHandles.find(vsHash);
			if (it != ms_shaderHandles.end()) {
				ASSERT(it->second.Count);
				--it->second.Count;
				if (!it->second.Count) {
					::bgfx::ShaderHandle handle({ it->second.Handle });
					::bgfx::destroy(handle);
					ms_shaderHandles.erase(it);
				}
			}
		}

		{
			CHashString fsHash(m_fsPath.c_str());
			const std::unordered_map<CHashString, FShaderData>::iterator& it = ms_shaderHandles.find(fsHash);
			if (it != ms_shaderHandles.end()) {
				ASSERT(it->second.Count);
				--it->second.Count;
				if (!it->second.Count) {
					::bgfx::ShaderHandle handle({ it->second.Handle });
					::bgfx::destroy(handle);
					ms_shaderHandles.erase(it);
				}
			}
		}

		{
			CHashString csHash(m_csPath.c_str());
			const std::unordered_map<CHashString, FShaderData>::iterator& it = ms_shaderHandles.find(csHash);
			if (it != ms_shaderHandles.end()) {
				ASSERT(it->second.Count);
				--it->second.Count;
				if (!it->second.Count) {
					::bgfx::ShaderHandle handle({ it->second.Handle });
					::bgfx::destroy(handle);
					ms_shaderHandles.erase(it);
				}
			}
		}

		::bgfx::ProgramHandle handle = { m_program };
		BGFXDESTROY(handle);
		m_program = handle.idx;
	}

	void CBgfxGraphicsMaterial::getPlatformPath(std::string& s)
	{
		std::string platform;
#ifdef DA_PLATFORM_WINDOWS
		platform = "windows";
#elif defined(DA_PLATFORM_MACOSX)
		platform = "macosx";
#elif defined (DA_PLATFORM_LINUX)
		platform = "linux";
#endif

		switch (CbgfxGraphicsApi::getRendererApi())
		{
		case ERenderApis::D3D9:
		case ERenderApis::D3D11:
		case ERenderApis::D3D12:
			s.append(".dx");
			break;
		case ERenderApis::Metal:
			s.append(".mt");
			break;
		case ERenderApis::Vulkan:
			s.append(".vk");
			break;
		case ERenderApis::OpenGL:
			s.append(".gl");
			break;
		}

		for (size_t i = s.size() - 1; i >= 0; i--) {
			if (s[i] == '\\' || s[i] == '/') {
				s.insert(i + 1, platform + "/");
				break;
			}
		}
	}


	uint16_t CBgfxGraphicsMaterial::getHandle() const
	{
		return m_program;
	}

	CBgfxGraphicsMaterial::~CBgfxGraphicsMaterial()
	{
		shutdown();
	}

}

#endif

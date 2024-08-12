#include "texture_loader.h"
#include "../log.hpp"
#include <errno.h>
#include <algorithm>


nvtt::Context CTextureLoader::ms_context(true);
std::unordered_map<da::core::CGuid, FAssetData> CTextureLoader::ms_textureSaved;

CTextureLoader::CTextureLoader(const std::string& path, const std::string& targetPath, const std::string& name) 
	: m_path(path), m_targetPath(targetPath), m_name(name)
{

}

CTextureLoader::CTextureLoader(const std::string& path, const std::string& name, const std::string& targetPath, uint32_t width, uint32_t height, uint8_t* data)
{
	const uint32_t channels = 4;
	const uint32_t bytesPerChannel = 1;
	
	m_name = name;
	m_path = path;

	for (uint32_t i = 0; i < m_name.size(); i++) {
		if (m_name[i] == '\\' || m_name[i] == '/') {
			m_name.erase(m_name.begin(), m_name.begin() + i + 1);
			break;
		}
	}

	for (uint32_t i = 0; i < m_name.size(); i++) {
		if (m_name[i] == '.') {
			m_name.erase(m_name.begin()+i, m_name.end());
			break;
		}
	}

	m_dataHash = HASHSZ(name.data(), name.size());
	m_hash = da::core::CGuid::Generate(m_dataHash);

	m_targetPath = targetPath + m_hash.c_str() + std::string(".dds");
	if (!m_surface.loadFromMemory(data, width, &m_hasAlpha))
	{
		da::cout << "Failed to load?" << da::endl;
	}
}

CTextureLoader::CTextureLoader() : m_hash()
{

}

CTextureLoader::~CTextureLoader()
{

}

bool CTextureLoader::loadTexture()
{
	FILE* f = fopen(m_path.c_str(), "r");
	if (!f)
	{
		return false;
	}

	fseek(f, 0L, SEEK_END);
	size_t sz = ftell(f);
	char* ch = new char[sz];
	rewind(f);
	for (size_t i = 0; i < sz; i++) {
		ch[i] = fgetc(f);
		if (feof(f)) break;
	}

	fclose(f);

	m_dataHash = HASHSZ(m_name.data(), m_name.size());
	m_hash  = da::core::CGuid::Generate(m_dataHash);
	std::string path = m_hash.c_str();

	m_targetPath = m_targetPath + path + ".dds";

	if (!m_surface.load(m_path.c_str(), &m_hasAlpha))
	{
		return false;
	}

	return true;
}

bool CTextureLoader::saveTexture()
{
	if (ms_textureSaved.find(m_hash) != ms_textureSaved.end())
	{
		da::cout << "Failed to save file: Already Exists..." << da::endl;
		return false;
	}

	da::cout << "[Mips " << std::to_string(m_surface.countMipmaps()) << " ]Writing Image Asset : " << m_path << " To : " << m_targetPath << da::endl;

	nvtt::CompressionOptions compressionOptions;
	compressionOptions.setFormat(nvtt::Format_BC7);

	nvtt::OutputOptions outputOptions;
	outputOptions.setSrgbFlag(true);
	outputOptions.setFileName(m_targetPath.c_str());
	
	if (!ms_context.outputHeader(m_surface, m_surface.countMipmaps() /* number of mipmaps */, compressionOptions, outputOptions)) {
		da::cout << "Failed to save file: Cannot output header..." << da::endl;
		return false;
	}

	for (int mip = 0; mip < m_surface.countMipmaps(); mip++) {
		// Compress this image and write its data.
		if (!ms_context.compress(m_surface, 0 /* face */, mip, compressionOptions, outputOptions)) {
			da::cout << "Failed to save file: Cannot compress..." << da::endl;
			return false;
		}

		if (mip == m_surface.countMipmaps() - 1) break;

		// Prepare the next mip:

		/*// Convert to linear premultiplied alpha. Note that toLinearFromSrgb()
		// will clamp HDR images; consider e.g. toLinear(2.2f) instead.
		surface.toLinearFromSrgb();
		surface.premultiplyAlpha();*/

		// Resize the image to the next mipmap size.
		// NVTT has several mipmapping filters; Box is the lowest-quality, but
		// also the fastest to use.
		m_surface.buildNextMipmap(nvtt::MipmapFilter_Box);
		// For general image resizing. use image.resize().

		int size = ms_context.estimateSize(m_surface, m_surface.countMipmaps(), compressionOptions);

		da::cout << "[Mips " << std::to_string(m_surface.countMipmaps()) << " ]Writing Image Asset : " << m_path << " To : " << m_targetPath << " Estimated size: " << std::to_string(size) << da::endl;
		/*// Convert back to unpremultiplied sRGB.
		surface.demultiplyAlpha();
		surface.toSrgb();*/
	}

	
	ms_textureSaved[m_hash] = { m_path, m_name, m_targetPath, m_dataHash };
}

const std::string& CTextureLoader::getTargetPath() const
{
	return m_targetPath;
}

const std::string& CTextureLoader::getName() const
{
	return m_name;
}

const da::core::CGuid &CTextureLoader::getHash() const
{
	return m_hash;
}

const uint32_t CTextureLoader::getDataHash() const
{
	return m_dataHash;
}

const std::unordered_map<da::core::CGuid, FAssetData>& CTextureLoader::getTextures()
{
	return ms_textureSaved;
}


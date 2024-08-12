#pragma once
#define DA_REVIEW
#include "stl/guid.h"
#include "stl/hashstring.h"
#include <string>
#include "nvtt/nvtt.h"
#include <unordered_map>

struct FAssetData
{
	std::string OgPath;
	std::string Name;
	std::string Path;
	uint32_t DataHash;
};


class CTextureLoader
{
public:
	CTextureLoader();
	CTextureLoader(const std::string& path, const std::string& targetPath, const std::string& name);
	CTextureLoader(const std::string& path, const std::string& name, const std::string& targetPath, uint32_t width, uint32_t height, uint8_t* data);
	~CTextureLoader();
	bool loadTexture();
	bool saveTexture();

	const std::string& getTargetPath() const;
	const std::string& getName() const;
	const da::core::CGuid& getHash() const;
	const uint32_t getDataHash() const;
	static const std::unordered_map<da::core::CGuid, FAssetData>& getTextures();

private:
	std::string m_path, m_targetPath, m_name;
	uint32_t m_dataHash;
	da::core::CGuid m_hash;
	bool m_hasAlpha;
	nvtt::Surface m_surface;
	static nvtt::Context ms_context;
	static std::unordered_map <da::core::CGuid , FAssetData > ms_textureSaved;
};
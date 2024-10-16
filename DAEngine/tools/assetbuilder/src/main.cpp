
#define DA_REVIEW
#include <iostream>
#define HAVE_CUDA
#include "nvtt/nvtt.h"
#include "stl/hashstring.h"
#include "log.hpp"
#include "loader/texture_loader.h"
#include "loader/model_loader.h"
#include "threadpool.h"


#include <filesystem>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream> 
#include <cstdio>
#include <array>
#include <ostream>

#define ASSET_CACHE_PATH "asset_cache.bin"

using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

int main(int argc, const char* argv[])
{
	if (argc <= 2) {
		da::cout << "Please supply asset directory in args" << da::endl;
		return -1;
	}


	typedef const const std::pair<std::string, std::map<CHashString, FAssetData>*> AssetDataType;

	AssetDataType dataAssets[] = {
		  { "texture", const_cast<std::map<CHashString, FAssetData>*>(&CTextureLoader::getTextures())}
		, { "model", &CModelLoader::ms_modelSaved }
		, { "skeleton", &CModelLoader::ms_skeleSaved }
		, { "animation", &CModelLoader::ms_AnimSaved }
		, { "material", &FMaterial::ms_materialSaved }
	};

	const std::filesystem::path p(argv[1]);
	const std::filesystem::path enumPath(argv[2]);

	da::cout << p.string() << da::endl;
	da::cout << enumPath.string() << da::endl;

	da::cout.stream.open((p.string() + "/output.txt").c_str(), std::ofstream::out);

	uint64_t lastRun = 0;
	if (std::filesystem::exists(ASSET_CACHE_PATH))
	{
		da::cout << "FOUND ASSET CACHE" << da::endl;
		std::fstream in;
		in.open(ASSET_CACHE_PATH, std::ios::in | std::ios::binary);
		std::unordered_map< CHashString, FAssetData> result;
		in.read((char*)&lastRun , sizeof(uint64_t));
		for (size_t i = 0; i < sizeof(dataAssets) / sizeof(AssetDataType); i++)
		{
			*dataAssets[i].second = FAssetData::deserialize(in, lastRun);
		}
		
		in.close();

		da::cout << "ASSET CACHE: " << std::to_string(lastRun) << da::endl;
	}

	uint64_t newTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	
	std::string outPath = p.string();
	auto it = outPath.find("source");
	if (it == -1)
	{
		return -1;
	}

	outPath.erase(it, 7);

	for (const auto& dirEntry : recursive_directory_iterator(p)) {
		if (dirEntry.is_directory()) {
			continue;
		}

		uint64_t writeTime = to_time_t(dirEntry.last_write_time());
		if (writeTime <= lastRun) {

			if (FAssetData::s_allFiles.find(dirEntry.path().string()) != FAssetData::s_allFiles.end())
			{
				da::cout << "Skipping: " << dirEntry.path().string() << da::endl;
				continue;
			}
		}

		//da::cout << dirEntry.path() << da::endl;
		std::string extStr = dirEntry.path().extension().string();
		CHashString ext = CHashString(extStr.c_str(), extStr.size());
		std::string name = dirEntry.path().filename().string();
		name.erase(name.end() - extStr.size(), name.end());
		std::string exPath = dirEntry.path().string();
		size_t fSize = exPath.find("source");
		exPath.erase(0, fSize+sizeof("source"));
		fSize = exPath.find(dirEntry.path().filename().string());
		exPath.erase(fSize, dirEntry.path().filename().string().size());


		std::string dir = outPath;
		uint64_t hash = ext.hash();
		switch (hash)
		{
		case HASH(".jpg"):
		case HASH(".jpeg"):
		case HASH(".png"):
		{
			da::cout << "Found Image Asset: " << dirEntry.path().string() << da::endl;

			dir += exPath;
			std::filesystem::create_directories(dir);
			dir += dirEntry.path().filename().string();
			std::string dir1 = dir;
			size_t f = dir1.find(dirEntry.path().filename().string());
			dir1.erase(f, dirEntry.path().filename().string().size());
			dir.erase(dir.end() - extStr.size(), dir.end());
			dir += std::string(".dds");

			CThreadPool::get().ms_threads.push_back(std::thread([dirEntry, dir, name] {
				CTextureLoader texture(dirEntry.path().string(), dir, name);
				if (texture.loadTexture())
				{
					if (!texture.saveTexture())
					{
						da::cout << "Failed to save file: " << dir;
					}
				}
			}));
			break;
		}
		case HASH(".fbx"):
		{
			dir += exPath;
			std::filesystem::create_directories(dir);
			dir += dirEntry.path().filename().string();
			std::string dir1 = dir;
			size_t f = dir1.find(dirEntry.path().filename().string());
			dir1.erase(f, dirEntry.path().filename().string().size());
			dir.erase(dir.end() - extStr.size(), dir.end());
			
			CThreadPool::get().ms_threads.push_back(std::thread([dirEntry, dir, dir1, name] {
				CModelLoader model = CModelLoader(dirEntry.path().string(), dir, dir1, name);
				if (model.loadModel())
				{
					model.saveModel();
				}
			}));
		}
		default:
		{
			break;
		}
		}
	}

	CThreadPool::get().wait();
	std::filesystem::create_directories(enumPath);

	auto enumCreation = [&enumPath](const std::string& enumName, std::map<CHashString, FAssetData>& data)
	{
		std::ofstream outEnum;
		outEnum.open((enumPath.string() + enumName + std::string(".generated.h")).c_str(), std::ios::out | std::ios::trunc);

		std::string enumClassName = enumName;
		enumClassName.insert(0, "E");
		enumClassName[1] = enumClassName[1] - 32;

		outEnum << "/*******************************************************\n";
		outEnum << " *                                                     *\n";
		outEnum << " *    This enum has been automatically generated by    *\n";
		outEnum << " *               the AssetBuilder tool.                *\n";
		outEnum << " *                                                     *\n";
		outEnum << " *    Do not modify this code manually, as changes     *\n";
		outEnum << " *    may be overwritten during the next build.        *\n";
		outEnum << " *                                                     *\n";
		outEnum << " *    To modify the contents of this enum, please      *\n";
		outEnum << " *    update the source data in the AssetBuilder       *\n";
		outEnum << " *    configuration.                                   *\n";
		outEnum << " *                                                     *\n";
		outEnum << " *******************************************************/\n";



		outEnum << "#pragma once" << std::endl << std::endl;
		outEnum << "enum class " <<  enumClassName << " : unsigned long long" << std::endl;
		outEnum << "{" << std::endl;

		std::vector<CHashString> to_remove = {};

		for (const auto& kv : data) {

			if (!std::filesystem::exists(kv.second.OgPath))
			{
				to_remove.emplace_back(kv.first.c_str());
				continue;
			}

			outEnum << "\t";
			outEnum << "/*" << std::endl;
			outEnum << "\t\t" << kv.second.Path << " HASH(" << HASH(kv.second.Path.c_str(), kv.second.Path.size()) << ")" << std::endl;
			outEnum << "\t" " */" << std::endl;
			outEnum << "\t" << kv.second.Name << " = " << HASH(kv.second.Path.c_str(), kv.second.Path.size()) << "," << std::endl << std::endl;
		}

		outEnum << "};" << std::endl;

		outEnum.close();

		for (const CHashString& h : to_remove)
		{
			const std::map<CHashString, FAssetData>::iterator& it = data.find(h);
			if (it != data.end())
			{
				data.erase(it);
			}
		}

	};


	std::ostringstream ss;
	ss.write((char*)&newTime, sizeof(uint64_t));
	for (size_t i = 0; i < sizeof(dataAssets) / sizeof(AssetDataType); i++)
	{
		enumCreation(dataAssets[i].first, const_cast<std::map<CHashString, FAssetData>&>(*dataAssets[i].second));
		FAssetData::serialize(ss, *dataAssets[i].second, newTime);
	}

	{
		std::ofstream out;
		out.open(ASSET_CACHE_PATH, std::ios::out | std::ios::trunc | std::ios::binary);
		out.write(ss.str().c_str(), ss.str().size());
		out.close();
	}

	uint64_t elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() - newTime;
	da::cout << "Elapsed Time: " << std::to_string(elapsedTime) << " seconds" << da::endl;

	return 0;
}


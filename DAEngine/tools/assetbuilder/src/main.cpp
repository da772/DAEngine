
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

using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

int main(int argc, const char* argv[])
{
	if (argc <= 2) {
		da::cout << "Please supply asset directory in args" << da::endl;
		return -1;
	}

	const std::filesystem::path p(argv[1]);
	const std::filesystem::path enumPath(argv[2]);

	da::cout << p.string() << da::endl;
	da::cout << enumPath.string() << da::endl;

	da::cout.stream.open((p.string() + "/output.txt").c_str(), std::ofstream::out);

	std::string timeStampPath = p.string() + "/timestamp.txt";
	uint64_t lastRun = 0;
	if (std::filesystem::exists(timeStampPath))
	{
		std::ifstream infile(timeStampPath);

		if (infile.good())
		{
			std::string sLine;
			getline(infile, sLine);
			lastRun = std::stoull(sLine);
		}

		infile.close();
	}

	uint64_t newTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	std::ofstream timeStamp(timeStampPath, std::ios::trunc);
	timeStamp << std::to_string(newTime);
	timeStamp.close();

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

		//da::cout << dirEntry.path() << da::endl;
		std::string extStr = dirEntry.path().extension().string();
		CHashString ext = CHashString(extStr.c_str(), extStr.size());
		std::string name = dirEntry.path().filename().string();
		name.erase(name.end() - extStr.size(), name.end());

		std::string dir = outPath;
		switch (ext.hash())
		{
		case HASH(".jpg"):
		case HASH(".jpeg"):
		case HASH(".png"):
		{
			dir += std::string("Textures\\");
			da::cout << "Found Image Asset: " << dirEntry.path().string() << da::endl;
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
			dir += std::string("Props\\") + dirEntry.path().filename().string();
			dir.erase(dir.end() - extStr.size(), dir.end());
			std::filesystem::create_directories(outPath);
			dir += std::string(".prp");
			CThreadPool::get().ms_threads.push_back(std::thread([dirEntry, dir, outPath, name] {
				CModelLoader model = CModelLoader(dirEntry.path().string(), dir, outPath, name);
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


		uint64_t writeTime = to_time_t(dirEntry.last_write_time());
		if (writeTime <= lastRun) {
			da::cout << "Skipping: " << dirEntry.path().string() << da::endl;
			continue;
		}
	}

	std::filesystem::create_directories(enumPath);

	std::ofstream out;
	out.open((enumPath.string() + std::string("map.txt")).c_str(), std::ios::out | std::ios::trunc);

	out << "MATERIALS:" << std::endl;

	for (const auto& kv : FMaterial::ms_materialSaved) {
		out << kv.first.c_str() << " : " << kv.second.Name << " : " << kv.second.OgPath << " : " << kv.second.Path << std::endl;
	}

	out << "TEXTURES:" << std::endl;

	for (const auto& kv : CTextureLoader::getTextures()) {
		out << kv.first.c_str() << " : " << kv.second.Name << " : " << kv.second.OgPath << " : " << kv.second.Path << std::endl;
	}

	out.close();

	CThreadPool::get().wait();
	

	{

		std::ofstream outEnum;
		outEnum.open((enumPath.string() + std::string("texture.generated.h")).c_str(), std::ios::out | std::ios::trunc);

		outEnum << "#pragma once" << std::endl << std::endl;
		outEnum << "enum class Texture : unsigned int" << std::endl;
		outEnum << "{" << std::endl;

		for (const auto& kv : CTextureLoader::getTextures()) {
			outEnum << "\t";
			outEnum << "/*" << kv.first.c_str() << " : " << kv.second.Name << " : " << kv.second.OgPath << " : " << kv.second.Path << "*/" << std::endl;
			outEnum << "\t" << kv.second.Name << " = " << std::to_string(kv.second.DataHash) << "," << std::endl << std::endl;
		}

		outEnum << "};" << std::endl;

		outEnum.close();
	}

	{

		std::ofstream outEnum;
		outEnum.open((enumPath.string() + std::string("model.generated.h")).c_str(), std::ios::out | std::ios::trunc);

		outEnum << "#pragma once" << std::endl << std::endl;
		outEnum << "enum class Model : unsigned int" << std::endl;
		outEnum << "{" << std::endl;

		for (const auto& kv : CModelLoader::ms_modelSaved) {
			outEnum << "\t";
			outEnum << "/*" << kv.first.c_str() << " : " << kv.second.Name << " : " << kv.second.OgPath << " : " << kv.second.Path << "*/" << std::endl;
			outEnum << "\t" << kv.second.Name << " = " << std::to_string(kv.second.DataHash) << "," << std::endl << std::endl;
		}

		outEnum << "};" << std::endl;
		outEnum.close();
	}

	uint64_t elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() - newTime;
	da::cout << "Elapsed Time: " << std::to_string(elapsedTime) << " seconds" << da::endl;

	return 0;
}


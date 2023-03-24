#include <iostream>
#include <shaderc.h>

#include <filesystem>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream> 

using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

enum class EShaderTypes : uint8_t {
	OpenGL,
	OpenGLES,
	DirectX,
	Vulkan,
	Metal,
	PS,
	COUNT
};

enum class EPlatformTypes : uint8_t {
	Android,
	WASM,
	iOS,
	Linux,
	Orbis,
	MacOSX,
	Windows,
	COUNT
};

int GenerateShader(std::vector<std::string> args, const std::filesystem::directory_entry& entry, EPlatformTypes platformType, EShaderTypes shaderType);

template<typename TP>
std::time_t to_time_t(TP tp) {
    using namespace std::chrono;
    auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now() + system_clock::now());
    return system_clock::to_time_t(sctp);
}

std::vector<std::vector<EShaderTypes>> s_platformShaderTypes = {
	{EShaderTypes::OpenGLES}, // Android
	{EShaderTypes::OpenGLES}, // WASM
	{EShaderTypes::Metal, EShaderTypes::OpenGLES}, // iOS
	{EShaderTypes::Vulkan, EShaderTypes::OpenGL}, // Linux
	{},//{EShaderTypes::PS}, // ORBIS
	{EShaderTypes::Vulkan, EShaderTypes::OpenGL, EShaderTypes::Metal}, // OSX
	{EShaderTypes::Vulkan, EShaderTypes::OpenGL, EShaderTypes::DirectX}, // Windows

};

const char* s_shaderName[] = {
	"gl",
	"gles",
	"dx",
	"vk",
	"mt",
	"pssl"
};

const char* s_shaderPlatform[] = {
	"440",
	"320_es",
	"s_5_0",
	"spirv",
	"metal",
	"pssl"
};

const char* s_platformName[] = {
	"android",
	"wasm",
	"ios",
	"linux",
	"orbis",
	"osx",
	"windows"
};


const char* s_platformDisplayName[] = {
	"android",
	"wasm",
	"ios",
	"linux",
	"psx",
	"macosx",
	"windows"
};


int main(int argc, const char* argv[])
{
	if (argc <= 1) {
		std::cout << "Please supply shader directory in args" << std::endl;
		return -1;
	}

	const std::filesystem::path p(argv[1]);
	
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

	for (const auto& dirEntry : recursive_directory_iterator(p)) {
		if (dirEntry.is_directory()) {
			continue;
		}

		//std::cout << dirEntry.path() << std::endl;
		if (dirEntry.path().extension() != ".sc") continue;
		if (dirEntry.path().string().find("varying.def.sc") != std::string::npos) continue;
		std::cout << "Found: " << dirEntry.path().string() << std::endl;
		uint64_t writeTime = to_time_t(dirEntry.last_write_time());
		if (writeTime <= lastRun) {
			std::cout << "Skipping: " << dirEntry.path().string() << std::endl;
			continue;
		}

		for (int i = 0; i < (int)EPlatformTypes::COUNT; i++) {
			std::string folderPath = dirEntry.path().string().substr(0, dirEntry.path().string().size() - dirEntry.path().filename().string().size()) + s_platformDisplayName[i];
			std::filesystem::create_directory(folderPath);
			std::cout << "Creating Directory for: " << dirEntry.path().string() << " AT: " << folderPath << std::endl; 
			for (EShaderTypes shaderType : s_platformShaderTypes[i])
			{
				GenerateShader({"-i", argv[1]}, dirEntry, (EPlatformTypes)i, shaderType);
			}
		}
		
	}

	return 0;
}

int GenerateShader(std::vector<std::string> args, const std::filesystem::directory_entry& entry, EPlatformTypes platformType, EShaderTypes shaderType)
{

#ifndef _WIN32
	if (shaderType == EShaderTypes::DirectX) {
		std::cout << "WINDOWS ONLY: Failed to generate DX shader for " << entry.path() << std::endl;
		return -1;
	}
#endif

	std::string path = entry.path().string();
	std::string fileName = entry.path().filename().string();
	std::string folderPath = entry.path().string().substr(0, entry.path().string().size() - entry.path().filename().string().size()) + s_platformDisplayName[(int)platformType] + "/";

	std::string newPath = folderPath + fileName + "." + s_shaderName[(int)shaderType];

	args.push_back("-f");
	args.push_back(path);

	args.push_back("-o");
	args.push_back(newPath);

	args.push_back("--type");
	if (fileName.find("fs") != std::string::npos) {
		args.push_back("fragment");
	}
	else if (fileName.find("vs") != std::string::npos) {
		args.push_back("vertex");
	}
	else
	{
		std::cout << "Failed to generate shader for " << entry.path() << std::endl;
		return -1;
	}

	args.push_back("--platform");

	if (shaderType == EShaderTypes::Vulkan && platformType == EPlatformTypes::MacOSX) {
		args.push_back(s_platformName[(int)EPlatformTypes::Windows]);
	}
	else {
		args.push_back(s_platformName[(int)platformType]);
	}

	args.push_back("--profile");
	args.push_back(s_shaderPlatform[(int)shaderType]);


	int totalSize = 0;
	char** argv = new char*[args.size()];
	for (int i = 0; i < args.size(); i++) {
		char* c = new char[args[i].size() + 1];
		memcpy(c, args[i].c_str(), sizeof(char) * args[i].size());
		c[args[i].size()] = 0;
		argv[i] = c;
	}

	int result = bgfx::compileShader(args.size(), (const char**)argv);

	for (int i = 0; i < args.size(); i++) {
		delete argv[i];
	}

	delete[] argv;

	if (result != 0) {
		std::cout << "FAILED TO CREATE SHADER " << newPath << std::endl;
	}

	return result;
	
}


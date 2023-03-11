#include <iostream>
#include <shaderc.h>

#include <filesystem>
#include <vector>

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
	OSX,
	Windows,
	COUNT
};

int GenerateShader(std::vector<std::string> args, const std::filesystem::directory_entry& entry, EPlatformTypes platformType, EShaderTypes shaderType);


std::vector<std::vector<EShaderTypes>> s_platformShaderTypes = {
	{EShaderTypes::OpenGLES}, // Android
	{EShaderTypes::OpenGLES}, // WASM
	{EShaderTypes::Metal, EShaderTypes::OpenGLES}, // iOS
	{EShaderTypes::Vulkan, EShaderTypes::OpenGL}, // Linux
	{},//{EShaderTypes::PS}, // ORBIS
	{EShaderTypes::Vulkan, EShaderTypes::OpenGL, EShaderTypes::Metal}, // OSX
	{EShaderTypes::Vulkan, EShaderTypes::OpenGL, EShaderTypes::DirectX}, // Windows

};

char* s_shaderName[] = {
	"gl",
	"gles",
	"dx",
	"vk",
	"mt",
	"pssl"
};

char* s_shaderPlatform[] = {
	"440",
	"320_es",
	"s_5_0",
	"spirv",
	"metal",
	"pssl"
};

char* s_platformName[] = {
	"android",
	"wasm",
	"ios",
	"linux",
	"orbis",
	"osx",
	"windows"
};


int main(int argc, const char* argv[])
{
	if (argc <= 1) {
		std::cout << "Please supply shader directory in args" << std::endl;
		return -1;
	}

	for (const auto& dirEntry : recursive_directory_iterator(argv[1])) {
		if (dirEntry.is_directory()) {
			continue;
		}

		std::cout << dirEntry.path() << std::endl;
		if (dirEntry.path().extension() != ".sc") continue;
		if (dirEntry.path().string().find("varying.def.sc") != std::string::npos) continue;

		for (int i = 0; i < (int)EPlatformTypes::COUNT; i++) {
			std::string folderPath = dirEntry.path().string().substr(0, dirEntry.path().string().size() - dirEntry.path().filename().string().size()) + s_platformName[i];
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
	std::string path = entry.path().string();
	std::string fileName = entry.path().filename().string();
	std::string folderPath = entry.path().string().substr(0, entry.path().string().size() - entry.path().filename().string().size()) + s_platformName[(int)platformType] + "/";

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
#ifdef _WIN32
	if (shaderType == EShaderTypes::Vulkan && platformType == EPlatformTypes::OSX) {
		args.push_back(s_platformName[(int)EPlatformTypes::Windows]);
	}
	else {
		args.push_back(s_platformName[(int)platformType]);
	}
#else
	
	args.push_back(s_platformName[(int)platformType]);
#endif
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


#include <iostream>
#define HAVE_CUDA
#include "nvtt/nvtt.h"
#include "stl/hashstring.h"


#include <filesystem>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream> 
#include <cstdio>

using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

template<typename TP>
std::time_t to_time_t(TP tp) {
	using namespace std::chrono;
	auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now() + system_clock::now());
	return system_clock::to_time_t(sctp);
}

namespace da {

	static const char endl[] = "\n";

	class CStream {
	public:
		std::ofstream stream;
		inline CStream& operator<<(const char* str) {
			std::cout << str;
			stream << str;
			return *this;
		}
		inline CStream& operator<<(const std::string& str) {
			std::cout << str;
			stream << str;
			return *this;
		}
		inline CStream& operator<<(const std::filesystem::path& str) {
			std::cout << str;
			stream << str;
			return *this;
		}
	};

	static CStream cout;
}


int main(int argc, const char* argv[])
{
	if (argc <= 1) {
		da::cout << "Please supply asset directory in args" << da::endl;
		return -1;
	}

	const std::filesystem::path p(argv[1]);

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

	for (const auto& dirEntry : recursive_directory_iterator(p)) {
		if (dirEntry.is_directory()) {
			continue;
		}

		//da::cout << dirEntry.path() << da::endl;
		std::string path = dirEntry.path().string();
		std::string extStr = dirEntry.path().extension().string();
		CHashString ext = CHashString(extStr.c_str(), extStr.size());

		auto it = path.find("source");
		if (it == -1)
		{
			continue;
		}

		path.erase(it, 7);

		std::string dir = path;
		dir.erase(dir.size() - dirEntry.path().filename().string().size(), dir.size());
		std::filesystem::create_directories(dir);
		switch (ext.hash())
		{
			case HASH(".jpg"):
			case HASH(".jpeg"):
			case HASH(".png"):
			{
				da::cout << "Found Image Asset: " << dirEntry.path().string() << da::endl;
				int width, height, channels;
				nvtt::Context context(true);
				nvtt::Surface surface;

				bool hasAlpha;
				if (surface.load(dirEntry.path().string().c_str(), &hasAlpha))
				{
					path.erase(path.end() - extStr.size(), path.end());
					path += std::string(".dds");
					da::cout << "[Mips " << std::to_string(surface.countMipmaps()) << " ]Writing Image Asset : " << dirEntry.path().string() << " To : " << path << da::endl;

					nvtt::CompressionOptions compressionOptions;
					compressionOptions.setFormat(nvtt::Format_BC7);

					nvtt::OutputOptions outputOptions;
					outputOptions.setSrgbFlag(true);
					outputOptions.setFileName(path.c_str());

					if (!context.outputHeader(surface, surface.countMipmaps() /* number of mipmaps */, compressionOptions, outputOptions)) {
						std::cerr << "Writing the DDS header failed!";
						return 1;
					}

					for (int mip = 0; mip < surface.countMipmaps(); mip++) {
						// Compress this image and write its data.
						if (!context.compress(surface, 0 /* face */, mip, compressionOptions, outputOptions)) {
							std::cerr << "Compressing and writing the DDS file failed!";
							return 1;
						}

						if (mip == surface.countMipmaps() - 1) break;

						// Prepare the next mip:

						/*// Convert to linear premultiplied alpha. Note that toLinearFromSrgb()
						// will clamp HDR images; consider e.g. toLinear(2.2f) instead.
						surface.toLinearFromSrgb();
						surface.premultiplyAlpha();*/

						// Resize the image to the next mipmap size.
						// NVTT has several mipmapping filters; Box is the lowest-quality, but
						// also the fastest to use.
						surface.buildNextMipmap(nvtt::MipmapFilter_Box);
						// For general image resizing. use image.resize().

						int size = context.estimateSize(surface, surface.countMipmaps(), compressionOptions);

						da::cout << "[Mips " << std::to_string(surface.countMipmaps()) << " ]Writing Image Asset : " << dirEntry.path().string() << " To : " << path << " Estimated size: " << std::to_string(size) << da::endl;
						/*// Convert back to unpremultiplied sRGB.
						surface.demultiplyAlpha();
						surface.toSrgb();*/
					}
				}
				break;
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


	return 0;
}


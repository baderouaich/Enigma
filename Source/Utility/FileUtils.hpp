#pragma once
#ifndef ENIGMA_FILE_UTILS_H
#define ENIGMA_FILE_UTILS_H

#include <Core/Core.hpp>
#include <Logger/Logger.hpp>

#include <fstream>
// Configure std::filesystem for non MSVC compilers
#if defined(ENIGMA_PLATFORM_WINDOWS)
	#include <filesystem>
	namespace fs = std::filesystem;
#else
	#include <experimental/filesystem>
	namespace fs = std::experimental::filesystem;
#endif

NS_ENIGMA_BEGIN
class ENIGMA_API FileUtils
{
public:
	static bool Read(const String& filename, String& buffer)
	{
		if (std::ifstream ifs{ filename, std::ios::binary | std::ios::ate }) // ate: open at the end
		{
			const size_t file_size = static_cast<size_t>(ifs.tellg());
			buffer.resize(file_size, '\000');
			ifs.seekg(0, std::ios::beg);
			ifs.read(buffer.data(), file_size);
			ifs.close();
			return true;
		}
		else
		{
			ENIGMA_ERROR("Failed to read file {0}", filename);
			return false;
		}
	}

	static bool Write(const String& filename, const std::string_view& buffer)
	{
		if (std::ofstream ofs{ filename, std::ios::binary })
		{
			ofs.write(buffer.data(), buffer.size());
			ofs.close();
			return true;
		}
		else
		{
			ENIGMA_ERROR("Failed to write file {0}", filename);
			return false;
		}
	}


};
NS_ENIGMA_END

#endif // !ENIGMA_FILE_UTILS_H

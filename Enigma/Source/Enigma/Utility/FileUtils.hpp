#pragma once
#ifndef ENIGMA_FILE_UTILS_H
#define ENIGMA_FILE_UTILS_H

#include <Enigma/Core/Core.hpp>
#include <Enigma/Debug/Logger/Logger.hpp>

#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

NS_ENIGMA_BEGIN
class ENIGMA_API FileUtils
{
public:
	static bool Read(const String& filename, String& buffer)
	{
		if (std::ifstream ifs{ filename, std::ios::binary | std::ios::ate }) // ate: open & seek end 
		{
			size_t file_size = static_cast<size_t>(ifs.tellg());
			buffer.resize(file_size, '\000');
			ifs.seekg(0, std::ios::beg);
			ifs.read(buffer.data(), file_size);
			ifs.close();
			return true;
		}
		else
		{
			ENIGMA_CORE_ERROR("Failed to read file {0}", filename.c_str());
			return false;
		}
	}

	static bool Write(const String& filename, const StringView& buffer)
	{
		if (std::ofstream ofs{ filename, std::ios::binary })
		{
			ofs.write(buffer.data(), buffer.size());
			ofs.close();
			return true;
		}
		else
		{
			ENIGMA_CORE_ERROR("Failed to write file {0}", filename.c_str());
			return false;
		}
	}
};
NS_ENIGMA_END

#endif // !ENIGMA_FILE_UTILS_H

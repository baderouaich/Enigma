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


	/*
	*	Reads a file chunk by chunk
	*/
	static void ReadChunks(const String& filename, const size_t max_chunk_size, const std::function<void(std::vector<byte>&&)>& callback)
	{
		if (std::ifstream ifs{ filename, std::ios::binary }) 
		{
			while (!ifs.eof())
			{
				std::vector<std::uint8_t> chunk(max_chunk_size, '\000');
				ifs.read((char*)chunk.data(), chunk.size());

				// resize chunk if we read bytes less than max_chunk_size
				const auto bytes_read = ifs.gcount();
				if (bytes_read < max_chunk_size)
					chunk.resize(bytes_read);

				// serve chunk
				callback(std::move(chunk));
			}
			ifs.close();
		}
		else
		{
			ENIGMA_ERROR("Failed to read file chunks {0}", filename);
		}
	}

};
NS_ENIGMA_END

#endif // !ENIGMA_FILE_UTILS_H

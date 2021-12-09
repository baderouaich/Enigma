#include <pch.hpp>
#include "Config.hpp"
#include <Logger/Logger.hpp>

NS_ENIGMA_BEGIN

Config::Config(const fs::path& file_name)
	:
	m_ini_reader(new INIReader(file_name.string()))
{
	ENIGMA_ASSERT_OR_THROW(m_ini_reader, "Failed to initialize INIReader");

	if (m_ini_reader->ParseError() < 0) 
	{
		throw std::runtime_error("Couldn't load config file " + file_name.string());
	}
}

NS_ENIGMA_END

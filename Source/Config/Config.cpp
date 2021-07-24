#include <pch.hpp>
#include "Config.hpp"

NS_ENIGMA_BEGIN

Config::Config(const String& file_name)
	:
	m_ini_reader(new INIReader(file_name))
{
	ENIGMA_ASSERT_OR_THROW(m_ini_reader, "Failed to initialize INIReader");

	if (m_ini_reader->ParseError() < 0) 
	{
		throw std::runtime_error("Couldn't load config file " + file_name);
	}
}

NS_ENIGMA_END

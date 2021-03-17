#include <pch.hpp>
#include "Config.hpp"

NS_ENIGMA_BEGIN

Config::Config(const String& file_name) noexcept(false)
{
	m_ini_reader = std::make_unique<INIReader>(file_name);
	if (m_ini_reader->ParseError() < 0) 
	{
		throw std::runtime_error("Couldn't load config file " + file_name);
	}
}



Config::~Config() noexcept
{
}


NS_ENIGMA_END

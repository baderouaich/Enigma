#pragma once
#ifndef ENIGMA_CONFIG_H
#define ENIGMA_CONFIG_H

#include <Core/Core.hpp>
#include <INIReader.h>

/*
*	Config file Loader
*/
NS_ENIGMA_BEGIN
class ENIGMA_API Config
{
public:
	/*
	* @param file_name: config ini file name
	* @exception std::runtime_error on failure
	*/
	explicit Config(const String& file_name);
	~Config() noexcept = default;

public:
	// Integers (and make sure its not bool since its also considered integral: https://stackoverflow.com/questions/51859172/recognize-that-a-value-is-bool-in-a-template)
	template<typename T>
	inline typename std::enable_if<std::is_integral<T>::value && !std::is_same<T, bool>::value, T>::type
	Get(const String& section, const String& name, const T& default_value) const
	{
		return static_cast<T>(m_ini_reader->GetInteger(section, name, default_value));
	}
	
	// Floating points
	template<typename T>
	inline typename std::enable_if<std::is_floating_point<T>::value, T>::type
	Get(const String& section, const String& name, const T& default_value) const
	{
		return static_cast<T>(m_ini_reader->GetReal(section, name, default_value));
	}

	// Boolean
	template<typename T>
	inline typename std::enable_if<std::is_same<T, bool>::value , T>::type
	Get(const String& section, const String& name, const T& default_value) const
	{
		return m_ini_reader->GetBoolean(section, name, default_value);
	}

	// String
	template<typename T>
	inline typename std::enable_if<std::is_same<T, String>::value, T>::type
	Get(const String& section, const String& name, const T& default_value) const
	{
		return m_ini_reader->GetString(section, name, default_value);
	}

private:
	std::unique_ptr<INIReader> m_ini_reader{};

};
NS_ENIGMA_END

#endif // !ENIGMA_CONFIG_H

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
	* @throws std::runtime_error on failure
	*/
	Config(const String& file_name) noexcept(false);
	~Config() noexcept;

public: /* Function template specializations */

	// Other than floating point, integer, bool or String, just return default value.
	template<typename T>
	typename std::enable_if<
		!std::is_floating_point<T>::value &&
		!std::is_integral<T>::value  &&
		!std::is_same<T, String>::value,
		T>::type
	Get(const String& section, const String& name, const T& default_value) const
	{
		return default_value;
	}
	

	// Floating points
	template<typename F>
	typename std::enable_if<std::is_floating_point<F>::value && !std::is_integral<F>::value, F>::type
	Get(const String& section, const String& name, const F& default_value) const
	{
		return static_cast<F>(m_ini_reader->GetReal(section, name, default_value));
	}

	// Integers
	template<typename I>
	typename std::enable_if<std::is_integral<I>::value && !std::is_floating_point<I>::value, I>::type
	Get(const String& section, const String& name, const I& default_value) const
	{
		return static_cast<I>(m_ini_reader->GetInteger(section, name, default_value));
	}

	// String
	template<typename S>
	typename std::enable_if<std::is_same<S, String>::value, S>::type
	Get(const String& section, const String& name, const S& default_value) const
	{
		m_ini_reader->GetInteger(std::string_view("qsdsqdeq"), std::string_view("qsdsqdeq"), 64);
		return m_ini_reader->GetString(section, name, default_value);
	}


	// Bool
	template<>
	bool Get(const String& section, const String& name, const bool& default_value) const
	{
		return m_ini_reader->GetBoolean(section, name, default_value);
	}

private:
	std::unique_ptr<INIReader> m_ini_reader;

};
NS_ENIGMA_END

#endif // !ENIGMA_CONFIG_H


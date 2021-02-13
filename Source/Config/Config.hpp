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
	inline typename std::enable_if<
		!std::is_floating_point<T>::value &&
		!std::is_integral<T>::value &&
		!std::is_same<T, String>::value,
		T>::type
	 Get(const String& section, const String& name, const T& default_value) const;

	// Floating points
	template<typename F>
	inline typename std::enable_if<std::is_floating_point<F>::value && !std::is_integral<F>::value, F>::type
	Get(const String& section, const String& name, const F& default_value) const;

	
	// Integers
	template<typename I>
	inline typename std::enable_if<std::is_integral<I>::value && !std::is_floating_point<I>::value, I>::type
	Get(const String& section, const String& name, const I& default_value) const;


	// String
	template<typename S>
	inline typename std::enable_if<std::is_same<S, String>::value, S>::type
	Get(const String& section, const String& name, const S& default_value) const;


	// Bool
	template<>
	inline bool Get(const String& section, const String& name, const bool& default_value) const;

private:
	std::unique_ptr<INIReader> m_ini_reader;

};

// Impl

template<typename T>
inline typename std::enable_if<!std::is_floating_point<T>::value && !std::is_integral<T>::value && !std::is_same<T, String>::value, T>::type Config::Get(const String& section, const String& name, const T& default_value) const
{
	return default_value;
}

template<typename F>
inline typename std::enable_if<std::is_floating_point<F>::value && !std::is_integral<F>::value, F>::type Config::Get(const String& section, const String& name, const F& default_value) const
{
	return static_cast<F>(m_ini_reader->GetReal(section, name, default_value));
}

template<typename I>
inline typename std::enable_if<std::is_integral<I>::value && !std::is_floating_point<I>::value, I>::type Config::Get(const String& section, const String& name, const I& default_value) const
{
	return static_cast<I>(m_ini_reader->GetInteger(section, name, default_value));
}

template<typename S>
inline typename std::enable_if<std::is_same<S, String>::value, S>::type Config::Get(const String& section, const String& name, const S& default_value) const
{
	return m_ini_reader->GetString(section, name, default_value);
}

template<>
inline bool Config::Get(const String& section, const String& name, const bool& default_value) const
{
	return m_ini_reader->GetBoolean(section, name, default_value);
}

NS_ENIGMA_END

#endif // !ENIGMA_CONFIG_H

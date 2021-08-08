#pragma once
#ifndef ENIGMA_TRANSLATION_HPP
#define ENIGMA_TRANSLATION_HPP
#include <Core/Core.hpp>
#include <Config/Config.hpp>
#include <Utility/FileUtils.hpp>
#include <memory>
#include <string>
#include <spdlog/fmt/fmt.h> // fmt
#include <spdlog/fmt/bundled/format.h> // fmt::format

NS_ENIGMA_BEGIN

class ENIGMA_API Translation
{
	ENIGMA_STATIC_CLASS(Translation);
public:
	enum class Language : ui16
	{
		English = 0x00, // or None as Default
		French,

		__HALT,
		BEGIN = English,
		END = __HALT - 1
	};
	static String StringifyLanguageEnum(const Language l) noexcept;

public:
	static void Initialize(Language language = Language::English);

	// Saves current language to cache file to remember language in next run
	static void Shutdown();

public:
	static const String& Translate(const String& text);

	static void SetLanguage(const Language language);

public:
	static Language GetLanguage() noexcept { return m_language; }

public:
	static const std::unique_ptr<Config>& GetConfig() noexcept { return m_config; }

private:
	inline static std::unique_ptr<Config> m_config{ nullptr }; //< Translation config ini file dictionary
	inline static Language m_language{ Language::English }; //< Application current langugae
	inline static std::unordered_map<String, String> m_translation{}; //< Translation dictionary
	inline static std::list<String> m_english_texts; // < English texts from English.ini as default to help us translate to other languages
};

#define ENIGMA_TRANSLATE(text)						::Enigma::Translation::Translate(text)
#define ENIGMA_TRANSLATE_FMT(text, ...)				fmt::format(ENIGMA_TRANSLATE(text), __VA_ARGS__)
#define ENIGMA_TRANSLATE_CSTR(text)					ENIGMA_TRANSLATE(text).c_str()
#define ENIGMA_TRANSLATE_FMT_CSTR(text, ...)		ENIGMA_TRANSLATE_FMT(text, __VA_ARGS__).c_str()

NS_ENIGMA_END

#endif // !ENIGMA_TRANSLATION_HPP

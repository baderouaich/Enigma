#include "pch.hpp"
#include "Translation.hpp"

NS_ENIGMA_BEGIN
String Translation::StringifyLanguageEnum(const Language l) noexcept
{
#define CASE_RET_STR(c) case Language::c: return String(#c)
	switch (l)
	{
		CASE_RET_STR(French);
	default:
		CASE_RET_STR(English);
	}
#undef CASE_RET_STR
}

void Translation::Initialize(Language language)
{
	ENIGMA_TRACE_CURRENT_FUNCTION();

	// Get last saved language cache from last run if available, otherwise set to English.
	const fs::path current_translation_file = FileUtils::GetEnigmaExecutableDir() / Constants::Translation::TRANSLATION_CURRENT_FILE_PATH;
	if (fs::exists(current_translation_file) &&
		fs::is_regular_file(current_translation_file) &&
		!fs::is_empty(current_translation_file))
	{
		String lang_enum_id_str{};
		ui16 lang{};
		if (FileUtils::Read(current_translation_file, lang_enum_id_str); !lang_enum_id_str.empty() && std::isdigit(lang_enum_id_str[0]))
		{
			lang = static_cast<ui16>(std::stoi(lang_enum_id_str));
			if (ENIGMA_IS_BETWEEN(lang, ui16(Language::BEGIN), ui16(Language::END)))
			{
				language = static_cast<Language>(lang);
			}
		}
	}

	// Load default English texts
	const fs::path default_translation_file = FileUtils::GetEnigmaExecutableDir() / Constants::Translation::TRANSLATION_ENGLISH_PATH;
	if (fs::exists(current_translation_file) &&
		fs::is_regular_file(current_translation_file) &&
		!fs::is_empty(current_translation_file))
	{
		if (std::ifstream ifs{ default_translation_file })
		{
			while (!ifs.eof())
			{
				String line{};
				if (std::getline(ifs, line); !line.empty() && line[0] != ';')// skip comments & empty lines
				{
					m_english_texts.insert(line);
				}
			}
			ifs.close();
		}
	}
	else
	{
		ENIGMA_CRITICAL("Failed to load English.ini translation file");
		std::exit(EXIT_FAILURE);
	}

	// Set Current language
	SetLanguage(language);
}

void Translation::Shutdown()
{
	ENIGMA_TRACE_CURRENT_FUNCTION();

	const fs::path current_translation_file = FileUtils::GetEnigmaExecutableDir() / Constants::Translation::TRANSLATION_CURRENT_FILE_PATH;
#if defined(ENIGMA_DEBUG)
	ENIGMA_INFO("Saving current translation language enum id to {}...", current_translation_file.string());
#endif
	FileUtils::Write(current_translation_file, std::to_string(static_cast<ui16>(m_language)));

	m_translation.clear();
	m_english_texts.clear();
}

const String& Translation::Translate(const String& text)
{
	if (m_language == Language::English)
		return text; // text is in English by default
	if (m_translation.find(text) != m_translation.end()) // other language
		return m_translation[text];
	return text; // incase other language not found, return English text
}


void Translation::SetLanguage(const Language language)
{
	if (m_language == language) return;
	m_language = language;

	if (language == Language::English)
	{
		m_config.reset(); // if config is nullptr, its english by default
		return; // english is by default
	}

	// Load language config ini file
	const auto translation_dir = FileUtils::GetEnigmaExecutableDir() / Constants::Resources::RESOURCES_DIR / Constants::Translation::TRANSLATION_DIR;
	ENIGMA_INFO("Translation language: {}", StringifyLanguageEnum(language));
	const auto translation_ini_path = (translation_dir / StringifyLanguageEnum(language)).replace_extension(".ini").string();
	ENIGMA_INFO("Translation path: {}", translation_ini_path);
	m_config.reset(new Config(translation_ini_path));

	// Make translation dictionary of the current langugae from config file
	m_translation.clear();
	for (const String& english_text : m_english_texts)
	{
		m_translation[english_text] = m_config->Get<String>(StringifyLanguageEnum(m_language), english_text, english_text); // if translation not found, don't collapse! return english text instead.
	}
}

NS_ENIGMA_END

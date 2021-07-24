#pragma once
#include "Tool.hpp"
#include <Core/Core.hpp>
#include <imgui.h>

/*
*	Password Generator Tool Collapsing Header View To be drawn in ToolsScene
*/
NS_ENIGMA_BEGIN
class PasswordGeneratorTool : public Tool
{
public:
	PasswordGeneratorTool() noexcept = default;
	~PasswordGeneratorTool() noexcept = default;

public: /* Tool Life Cycle */
	void OnCreate() override;
	void OnDraw(Scene* parent) override;
	void OnDestroy() override;

public: /* Callbacks */
	void OnGenerateButtonPressed();
	void OnCopyPasswordButtonPressed();
	void OnCopyRememberPasswordSentenceButtonPressed();

private: // Password settings
	bool m_digits, m_uppercase_alphabets, m_lowercase_alphabets, m_special_characters;
	

	std::vector<const char*> m_lengths{}; // length combo box items (heap allocated elements with new[])
	i32 m_selected_length_index{0}; // selected length index from combo box

	//inline static constexpr size_t MAX_LENGTHS_ITEMS = 100; // max length combo box items
	//const char** m_lengths; // length combo box (heap allocated with new[])
	//i32 m_selected_length_index = 0; // selected length index from combo box
	
	String m_password{}; // generated password container
	String m_remember_password_sentence{}; // a helpful sentence to remember ur password based on password characters
	const std::unordered_map<char, String> m_remember_password_char_words
	{
		{'a', "alpha"},
		{'b', "beta"},
		{'c', "coffee" },
		{'d', "dinner" },
		{'e', "egg" },
		{'f', "flame" },
		{'g', "great" },
		{'h', "hello" },
		{'i', "indeed" },
		{'j', "jack" },
		{'k', "korea" },
		{'l', "lemon" },
		{'m', "man" },
		{'n', "no" },
		{'o', "ok" },
		{'p', "please" },
		{'q', "question" },
		{'r', "right" },
		{'s', "speak" },
		{'t', "tea" },
		{'u', "umbrella" },
		{'v', "visa" },
		{'w', "welcome" },
		{'x', "xbox" },
		{'y', "yes" },
		{'z', "zero" },

		{'A', "AMERICA"},
		{'B', "BRAVO"},
		{'C', "CANDY" },
		{'D', "DRONE" },
		{'E', "ERA" },
		{'F', "FACT" },
		{'G', "GARDIAN" },
		{'H', "HOLLY" },
		{'I', "INDIA" },
		{'J', "JAMES" },
		{'K', "KOALA" },
		{'L', "LUCK" },
		{'M', "MUSIC" },
		{'N', "NAP" },
		{'O', "OLIVE" },
		{'P', "PAINT" },
		{'Q', "QUEEN" },
		{'R', "RICE" },
		{'S', "SIR" },
		{'T', "TRACK" },
		{'U', "UNLESS" },
		{'V', "VARIABLE" },
		{'W', "WIND" },
		{'X', "XRAY" },
		{'Y', "YOUTUBE" },
		{'Z', "ZIP" },
	};
};
NS_ENIGMA_END

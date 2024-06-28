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
	PasswordGeneratorTool() = default;
	~PasswordGeneratorTool() = default;

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
	std::int32_t m_selected_length_index{0}; // selected length index from combo box

	//inline static constexpr std::size_t MAX_LENGTHS_ITEMS = 100; // max length combo box items
	//const char** m_lengths; // length combo box (heap allocated with new[])
	//std::int32_t m_selected_length_index = 0; // selected length index from combo box
	
	std::string m_password{}; // generated password container
	std::string m_remember_password_sentence{}; // a helpful sentence to remember ur password based on password characters
	inline static const std::unordered_map<char, std::string_view> m_remember_password_char_words // letter, word dictionary
	{
		{'a', "alpha"},
		{'b', "beta"},
		{'c', "coffee" },
		{'d', "delta" },
		{'e', "egg" },
		{'f', "flame" },
		{'g', "gamma" },
		{'h', "hello" },
		{'i', "indeed" },
		{'j', "jack" },
		{'k', "kind" },
		{'l', "lemon" },
		{'m', "man" },
		{'n', "new" },
		{'o', "omega" },
		{'p', "pi" },
		{'q', "question" },
		{'r', "right" },
		{'s', "sigma" },
		{'t', "tea" },
		{'u', "upsilon" },
		{'v', "visa" },
		{'w', "word" },
		{'x', "xbox" },
		{'y', "yield" },
		{'z', "zero" },

		{'A', "AMERICA"},
		{'B', "BRAVO"},
		{'C', "CANDY" },
		{'D', "DRONE" },
		{'E', "ERA" },
		{'F', "FACT" },
		{'G', "GARDIAN" },
		{'H', "HOLLY" },
		{'I', "ISLAND" },
		{'J', "JOKE" },
		{'K', "KOALA" },
		{'L', "LUCK" },
		{'M', "MUSIC" },
		{'N', "NASA" },
		{'O', "OLIVE" },
		{'P', "PAINT" },
		{'Q', "QUEEN" },
		{'R', "RAM" },
		{'S', "SPEED" },
		{'T', "TRACK" },
		{'U', "URANIUM" },
		{'V', "VARIABLE" },
		{'W', "WIND" },
		{'X', "XRAY" },
		{'Y', "YOUTUBE" },
		{'Z', "ZIP" },
	};
};
NS_ENIGMA_END

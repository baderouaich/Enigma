#pragma once
#include "Tool.hpp"
#include <Core/Core.hpp>
#include <imgui.h>

/*
*	Password Generator Tool Collapsing Header View To be drawn in ToolsScene
*/
NS_ENIGMA_BEGIN
class PasswordGeneratorTool : public Tool {
  public:
    PasswordGeneratorTool() = default;
    ~PasswordGeneratorTool() override = default;

  public: /* Tool Life Cycle */
    void OnCreate() override;
    void OnDraw(Scene *parent) override;
    void OnDestroy() override;

  public: /* Callbacks */
    void OnGenerateButtonPressed();
    void OnCopyPasswordButtonPressed();
    void OnCopyRememberPasswordSentenceButtonPressed();


  private: // Password settings
    bool m_digits{}, m_uppercase_alphabets{}, m_lowercase_alphabets{}, m_special_characters{};

    std::string m_length{"16"};
    std::string m_password{};                                                                     // generated password container
    std::string m_remember_password_sentence{};                                                   // a helpful sentence to remember ur password based on password characters
    inline static const std::unordered_map<char, std::string_view> m_remember_password_char_words // letter, word dictionary
      {
        {'a', "apple"},
        {'b', "banana"},
        {'c', "cat"},
        {'d', "dog"},
        {'e', "elephant"},
        {'f', "frog"},
        {'g', "grape"},
        {'h', "hat"},
        {'i', "ice"},
        {'j', "juice"},
        {'k', "kite"},
        {'l', "lion"},
        {'m', "moon"},
        {'n', "nest"},
        {'o', "orange"},
        {'p', "pizza"},
        {'q', "queen"},
        {'r', "rain"},
        {'s', "sun"},
        {'t', "tree"},
        {'u', "umbrella"},
        {'v', "violin"},
        {'w', "whale"},
        {'x', "x-ray"},
        {'y', "yogurt"},
        {'z', "zebra"},

        {'A', "Apple"},
        {'B', "Banana"},
        {'C', "Cat"},
        {'D', "Dog"},
        {'E', "Elephant"},
        {'F', "Frog"},
        {'G', "Grape"},
        {'H', "Hat"},
        {'I', "Ice"},
        {'J', "Juice"},
        {'K', "Kite"},
        {'L', "Lion"},
        {'M', "Moon"},
        {'N', "Nest"},
        {'O', "Orange"},
        {'P', "Pizza"},
        {'Q', "Queen"},
        {'R', "Rain"},
        {'S', "Sun"},
        {'T', "Tree"},
        {'U', "Umbrella"},
        {'V', "Violin"},
        {'W', "Whale"},
        {'X', "X-ray"},
        {'Y', "Yogurt"},
        {'Z', "Zebra"},
      };
};
NS_ENIGMA_END

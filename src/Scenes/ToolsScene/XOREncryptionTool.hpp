#pragma once
#include "Tool.hpp"
#include <Algorithm/Algorithm.hpp> //	Algorithm::Intent
#include <Core/Core.hpp>
#include <imgui.h>

/*
*	XOR Encryption/Decryption Tool Collapsing Header View To be drawn in ToolsScene
*/
NS_ENIGMA_BEGIN
class XOREncryptionTool : public Tool {
  public:
    XOREncryptionTool() = default;
    ~XOREncryptionTool() = default;

  public: /* Tool Life Cycle */
    void OnCreate() override;
    void OnDraw(Scene *parent) override;
    void OnDestroy() override;

  public: /* Callbacks */
    void OnEncryptButtonPressed();
    void OnDecryptButtonPressed();

  public:
    /** XOR Encrypts buffer string (with no padding)
    *	@param password: Encryption key
    *	@param buffer: Buffer string to encrypt
    *	@returns cipher (aka encrypted buffer)
    */
    std::string XOREncrypt(const std::string& password, const std::string& buffer);

    /** XOR Decrypts cipher string
    *	@param password: Encryption key
    *	@param cipher: Encrypted buffer (aka cipher)
    *	@returns recovered buffer
    */
    std::string XORDecrypt(const std::string& password, const std::string& cipher);

  private:
    Algorithm::Intent m_intent; // Are we Encryption or Decrypting?
    std::string m_password{};   // XOR Key/Password
    std::string m_input{};      // buffer/cipher to encrypt/decrypt with XOR
    std::string m_output{};     // buffer/cipher to encrypt/decrypt with XOR
};
NS_ENIGMA_END

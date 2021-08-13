#pragma once
#include "Tool.hpp"
#include <Core/Core.hpp>
#include <Algorithm/Algorithm.hpp> //	Algorithm::Intent
#include <imgui.h>

/*
*	XOR Encryption/Decryption Tool Collapsing Header View To be drawn in ToolsScene
*/
NS_ENIGMA_BEGIN
class XOREncryptionTool : public Tool
{
public:
	XOREncryptionTool() = default;
	~XOREncryptionTool() = default;

public: /* Tool Life Cycle */
	void OnCreate() override;
	void OnDraw(Scene* parent) override;
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
	String XOREncrypt(const String& password, const String& buffer)
	{
		// 0 & 1 => 1
		// 1 & 0 => 1
		// 0 & 0 => 0
		// 1 & 1 => 0
		String cipher = buffer;
		for (size_t i = 0; i < buffer.size(); i++) // loops and scrambles bits in the string
			cipher[i] = buffer[i] ^ password[i % password.size()];
		return cipher;
	}

	/** XOR Decrypts cipher string
	*	@param password: Encryption key
	*	@param cipher: Encyrpted buffer (aka cipher)
	*	@returns recovered buffer
	*/
	String XORDecrypt(const String& password, const String& cipher)
	{
		// Just redo XOR operation to recover buffer
		return XOREncrypt(password, cipher);
	}


private:
	Algorithm::Intent m_intent; // Are we Encryption or Decrypting?
	String m_password{}; // XOR Key/Password
	String m_input{}; // buffer/cipher to encrypt/decrypt with XOR
	String m_output{}; // buffer/cipher to encrypt/decrypt with XOR
};
NS_ENIGMA_END

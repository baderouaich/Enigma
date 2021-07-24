#pragma once
#include "Tool.hpp"
#include <Core/Core.hpp>
#include <imgui.h>

#include <cryptlib.h>
//#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <md5.h> 
#include <sha.h> 

/*
*	Hashing Tool Collapsing Header View To be drawn in ToolsScene
*/
NS_ENIGMA_BEGIN
class HashingTool : public Tool
{
	enum class HashAlgorithm : byte
	{
		MD5 = 0x00,
		SHA1,
		SHA244,
		SHA256,
		SHA384,
		SHA512,

		BEGIN = MD5,
		END = SHA512
	};	
	friend const char* operator *(const HashAlgorithm hash_algo) noexcept // stringify HashAlgorithm enum for radio buttons
	{
#define CASE_RET(c) case HashingTool::HashAlgorithm::c: return #c
		switch (hash_algo)
		{
			CASE_RET(MD5);
			CASE_RET(SHA1);
			CASE_RET(SHA244);
			CASE_RET(SHA256);
			CASE_RET(SHA384);
			CASE_RET(SHA512);
			default: return "<unknown hash algorithm>";
		};
#undef CASE_RET
	}
public:
	HashingTool() noexcept = default;
	~HashingTool() noexcept = default;

public: /* Tool Life Cycle */
	void OnCreate() override;
	void OnDraw(Scene* parent) override;
	void OnDestroy() override;

private: /* Hash Algorithms */
	HashAlgorithm m_selected_hash; // Radio buttons selected hash algorithm
	std::unique_ptr<CryptoPP::MD5> m_md5{nullptr};
	std::unique_ptr<CryptoPP::SHA1> m_sha1{nullptr};
	std::unique_ptr<CryptoPP::SHA224> m_sha244{nullptr};
	std::unique_ptr<CryptoPP::SHA256> m_sha256{nullptr};
	std::unique_ptr<CryptoPP::SHA384> m_sha384{nullptr};
	std::unique_ptr<CryptoPP::SHA512> m_sha512{nullptr};
};
NS_ENIGMA_END

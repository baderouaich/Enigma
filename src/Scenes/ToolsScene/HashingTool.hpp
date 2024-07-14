#pragma once
#include "Tool.hpp"
#include <Core/Core.hpp>
#include <imgui.h>

#include <cryptlib.h>
//#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <files.h>    // CryptoPP::FileSink
#include <hex.h>      // CryptoPP::HexEncoder
#include <keccak.h>   // Keccak
#include <md2.h>      // MD2
#include <md4.h>      // MD4
#include <md5.h>      // MD5
#include <sha.h>      // SHA1, SHA244, SHA256, SHA384, SHA512
#include <shake.h>    // Shake
#include <tiger.h>    // Tiger
#include <whrlpool.h> // Whirlpool
#include <sm3.h>      // SM3
#include <ripemd.h>   // RIPEMD128,RIPEMD160,RIPEMD256,RIPEMD320

/*
*	Hashing Tool Collapsing Header View To be drawn in ToolsScene
*/
NS_ENIGMA_BEGIN
class HashingTool : public Tool {
    enum class HashAlgorithm : byte {
      MD2 = 0x00,
      MD4,
      MD5,

      SHA1,
      SHA224,
      SHA256,
      SHA384,
      SHA512,

      SHAKE128,
      SHAKE256,

      KECCAK224,
      KECCAK256,
      KECCAK384,
      KECCAK512,

      RIPEMD128,
      RIPEMD160,
      RIPEMD256,
      RIPEMD320,

      TIGER,
      WHIRLPOOL,
      SM3,

      ENIGMA_ENUM_DECLARE_BEGIN_END(MD2)
    };
    friend const char *operator*(const HashAlgorithm hash_algo) noexcept // stringify HashAlgorithm enum for radio buttons
    {
#define CASE_RET(c)                   \
  case HashingTool::HashAlgorithm::c: \
    return #c
      switch (hash_algo) {
        CASE_RET(MD2);
        CASE_RET(MD4);
        CASE_RET(MD5);

        CASE_RET(SHA1);
        CASE_RET(SHA224);
        CASE_RET(SHA256);
        CASE_RET(SHA384);
        CASE_RET(SHA512);

        CASE_RET(SHAKE128);
        CASE_RET(SHAKE256);

        CASE_RET(KECCAK224);
        CASE_RET(KECCAK256);
        CASE_RET(KECCAK384);
        CASE_RET(KECCAK512);

        CASE_RET(RIPEMD128);
        CASE_RET(RIPEMD160);
        CASE_RET(RIPEMD256);
        CASE_RET(RIPEMD320);

        CASE_RET(TIGER);
        CASE_RET(WHIRLPOOL);
        CASE_RET(SM3);

        default:
          return "<unknown hash algorithm>";
      };
#undef CASE_RET
    }

  public:
    HashingTool() = default;
    ~HashingTool() = default;

  public: /* Tool Life Cycle */
    void OnCreate() override;
    void OnDraw(Scene *parent) override;
    void OnDestroy() override;

  private: /* Callbacks */
    void OnCalculateHashButtonPressed();
    void OnCopyHashButtonPressed();

  private: /* Hash Algorithms */
    enum class InputSource : byte {
      Text,
      File,
      ENIGMA_ENUM_DECLARE_BEGIN_END(Text)
    } m_input_source = InputSource::Text;
    static const char *inputSourceEnumToStr(InputSource is) {
      switch (is) {
        default:
        case InputSource::Text:
          return "Text";
        case InputSource::File:
          return "File";
      }
    }

    std::string m_input{};  // text to calculate hash for (Message) or input file
    std::string m_output{}; // calculated hash (Digest)

    HashAlgorithm m_selected_hash; // Radio buttons selected hash algorithm
    std::unique_ptr<CryptoPP::Weak::MD2> m_md2{nullptr};
    std::unique_ptr<CryptoPP::Weak::MD4> m_md4{nullptr};
    std::unique_ptr<CryptoPP::Weak::MD5> m_md5{nullptr};

    std::unique_ptr<CryptoPP::SHA1> m_sha1{nullptr};
    std::unique_ptr<CryptoPP::SHA224> m_sha224{nullptr};
    std::unique_ptr<CryptoPP::SHA256> m_sha256{nullptr};
    std::unique_ptr<CryptoPP::SHA384> m_sha384{nullptr};
    std::unique_ptr<CryptoPP::SHA512> m_sha512{nullptr};

    std::unique_ptr<CryptoPP::Keccak_224> m_keccak224{nullptr};
    std::unique_ptr<CryptoPP::Keccak_256> m_keccak256{nullptr};
    std::unique_ptr<CryptoPP::Keccak_384> m_keccak384{nullptr};
    std::unique_ptr<CryptoPP::Keccak_512> m_keccak512{nullptr};

    std::unique_ptr<CryptoPP::SHAKE128> m_shake128{nullptr};
    std::unique_ptr<CryptoPP::SHAKE256> m_shake256{nullptr};

    std::unique_ptr<CryptoPP::RIPEMD128> m_ripemd128{nullptr};
    std::unique_ptr<CryptoPP::RIPEMD160> m_ripemd160{nullptr};
    std::unique_ptr<CryptoPP::RIPEMD256> m_ripemd256{nullptr};
    std::unique_ptr<CryptoPP::RIPEMD320> m_ripemd320{nullptr};

    std::unique_ptr<CryptoPP::Tiger> m_tiger{nullptr};
    std::unique_ptr<CryptoPP::Whirlpool> m_whirlpool{nullptr};
    std::unique_ptr<CryptoPP::SM3> m_sm3{nullptr};

};
NS_ENIGMA_END

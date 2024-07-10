#include <pch.hpp>
#include "Algorithm.hpp"

#include "AES/AES.hpp"
#include "Blowfish/Blowfish.hpp"
#include "ChaCha20Poly1305/ChaCha20Poly1305.hpp"
#include "IDEA/IDEA.hpp"
#include "TripleDES/TripleDES.hpp"
#include "Twofish/Twofish.hpp"
#include "RSA/RSA.hpp"
//#include "RSA/RSA.hpp"
#include <Utility/Base64.hpp>
#include <Utility/FileUtils.hpp>
#include <Utility/StringUtils.hpp>

NS_ENIGMA_BEGIN

Algorithm::Algorithm(Type type, Intent intent) noexcept
    : m_type(type),
      m_intent(intent) {
  // we only need random seeder to generate random iv when encrypting
  if ((static_cast<bool>(intent & Intent::Encrypt) || m_type == Type::RSA) && !m_auto_seeded_random_pool)
    m_auto_seeded_random_pool = std::make_unique<CryptoPP::AutoSeededRandomPool>();
}

Algorithm::~Algorithm() noexcept {}


std::unique_ptr<Algorithm> Algorithm::CreateFromName(const std::string& algorithm_name, const Intent intent) {
  const std::string algo = StringUtils::LowerCopy(algorithm_name);

  const auto AlgoIn = [&algo](const std::vector<std::string_view>& v) noexcept -> bool {
    return std::find(v.begin(), v.end(), algo) != v.end();
  };

  if (AlgoIn({"aes", "aes-gcm", "aes_gcm", "aesgcm"}))
    return std::make_unique<AES>(intent);
  else if (AlgoIn({"chacha", "chacha20", "salsa", "salsa20", "chacha20poly1305", "salsapoly1305", "salsa20poly1305"}))
    return std::make_unique<ChaCha20Poly1305>(intent);
  else if (AlgoIn({"tripledes", "triple-des", "tripledes-eax", "tripledes_eax", "tripledeseax", "triple"}))
    return std::make_unique<TripleDES>(intent);
  else if (AlgoIn({"twofish", "twofish-gcm", "2fish", "twofish_gcm", "twofishgcm"}))
    return std::make_unique<Twofish>(intent);
  else if (AlgoIn({"idea", "idea-eax", "idea_eax", "ideaeax"}))
    return std::make_unique<IDEA>(intent);
  else if (AlgoIn({"blowfish", "blowfish-eax", "blowfish_eax", "blowfisheax"}))
    return std::make_unique<Blowfish>(intent);
  else if (AlgoIn({"rsa", "rsa-oaep", "rsa-oaep-sha256", "rsa_oaep", "rsa-sha256"}))
    return std::make_unique<RSA>(intent);
  else
    throw std::runtime_error("Unsupported algorithm: " + algorithm_name);
}

std::unique_ptr<Algorithm> Algorithm::CreateFromType(const Type type, const Intent intent) {
  return CreateFromName(AlgoTypeEnumToStr(type), intent);
}

std::vector<byte> Algorithm::GenerateRandomIV(const std::size_t size) {
  ENIGMA_ASSERT_OR_THROW(m_auto_seeded_random_pool, "Random seeder is not initialized properly");
  std::vector<byte> iv(size, '\000');
  m_auto_seeded_random_pool->GenerateBlock(iv.data(), iv.size());
  return iv;
}

std::string Algorithm::AlgoTypeEnumToStr(const Algorithm::Type e) noexcept {
#define CASE_RET(e)        \
  case Algorithm::Type::e: \
    return std::string(#e)
  switch (e) {
    CASE_RET(AES);
    CASE_RET(Twofish);
    CASE_RET(TripleDES);
    CASE_RET(Blowfish);
    CASE_RET(IDEA);
    CASE_RET(ChaCha20Poly1305);
    CASE_RET(RSA);
    default:
      return "<unknown algorithm>";
  }
#undef CASE_RET
}


std::string Algorithm::GetSupportedAlgorithmsStr() noexcept {
  std::string out = "[";
  for (byte i = static_cast<byte>(Algorithm::Type::BEGIN); i <= static_cast<byte>(Algorithm::Type::END); i++) {
    out += AlgoTypeEnumToStr(static_cast<Algorithm::Type>(i)) + (i != static_cast<byte>(Algorithm::Type::END) ? ", " : "");
  }
  out += ']';
  return out;
}

std::vector<std::pair<std::string, Algorithm::Type>> Algorithm::GetSupportedAlgorithms() noexcept {
  std::vector<std::pair<std::string, Algorithm::Type>> algos(static_cast<std::size_t>(Algorithm::Type::END));
  for (byte i = static_cast<byte>(Algorithm::Type::BEGIN); i <= static_cast<byte>(Algorithm::Type::END); i++) {
    algos[static_cast<std::size_t>(i) - 1] = std::make_pair(AlgoTypeEnumToStr(static_cast<Algorithm::Type>(i)), static_cast<Algorithm::Type>(i));
  }
  return algos;
}


NS_ENIGMA_END

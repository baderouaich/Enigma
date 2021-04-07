#include <pch.hpp>
#include "Algorithm.hpp"

#include "AES/AES.hpp"
#include "ChaCha20/ChaCha20.hpp"
#include "TripleDES/TripleDES.hpp"
#include "Twofish/Twofish.hpp"
#include "IDEA/IDEA.hpp"

NS_ENIGMA_BEGIN

Algorithm::Algorithm(Type type, Intent intent) noexcept
	:
	m_type(type),
	m_intent(intent)
{
	// we only generate random iv when encrypting
	if (intent == Intent::Encrypt)
		m_auto_seeded_random_pool = std::make_unique<CryptoPP::AutoSeededRandomPool>();
}

Algorithm::~Algorithm() noexcept
{
}


std::unique_ptr<Algorithm> Algorithm::CreateFromName(const String& mode, const Intent intent)
{
	const auto ModeIn = [&mode](const std::vector<std::string_view>& v) -> const bool
	{
		return std::find(v.begin(), v.end(), mode) != v.end();
	};

	if (ModeIn({ "aes", "aes-gcm" }))
		return std::make_unique<AES>(intent);
	else if (ModeIn({ "chacha", "chacha20", "salsa", "salsa20" }))
		return std::make_unique<ChaCha20>(intent);
	else if (ModeIn({ "tripledes", "triple-des", "tripledes-cbc" }))
		return std::make_unique<TripleDES>(intent);
	else if (ModeIn({ "twofish", "twofish-gcm" }))
		return std::make_unique<Twofish>(intent);
	else if (ModeIn({ "idea", "idea-cbc" }))
		return std::make_unique<IDEA>(intent);
	else
		throw std::runtime_error("Unsupported algorithm mode: " + mode);
}

std::unique_ptr<Algorithm> Algorithm::CreateFromType(const Type type, const Intent intent)
{
	String mode = AlgoTypeEnumToStr(type);
	StringUtils::Lower(mode);
	return CreateFromName(mode, intent);
}


String Algorithm::GenerateRandomIV(const size_t size)
{
	String iv(size, '\000');
	m_auto_seeded_random_pool->GenerateBlock(reinterpret_cast<byte*>(iv.data()), iv.size());
	return iv;
}

Algorithm::Type Algorithm::DetectFromCipherBase64(const String& cipher_base64)
{
	ENIGMA_ASSERT_OR_THROW(!cipher_base64.empty(), "Cannot auto-detect algorithm without cipher base64 text");

	// Decode base64 to cipher
	String cipher = Base64::Decode(cipher_base64);

	// check if successfully decoded
	ENIGMA_ASSERT_OR_THROW(!cipher.empty(), "Failed to decode cipher base64! please make sure you have the exact cipher base64 text you had in encryption");

	// extract first byte from cipher which must be the mode type used in encryption
	const byte cipher_first_byte = *cipher.begin();

	// Check if we detected a valid encryption algorithm mode
	ENIGMA_ASSERT_OR_THROW(ENIGMA_IS_BETWEEN(cipher_first_byte, static_cast<byte>(Algorithm::Type::First), static_cast<byte>(Algorithm::Type::Last)),
		"Could not auto-detect algorithm mode used for encryption");

	// if alles gut, return type
	return static_cast<Algorithm::Type>(cipher_first_byte);
}

Algorithm::Type Algorithm::DetectFromFile(const String& filename)
{
	// check if the infile exists
	ENIGMA_ASSERT_OR_THROW(!filename.empty(), "input file name is empty");
	ENIGMA_ASSERT_OR_THROW(fs::exists(filename), "input file does not exist");
	ENIGMA_ASSERT_OR_THROW(!fs::is_empty(filename), "input file is empty");
	ENIGMA_ASSERT_OR_THROW(fs::is_regular_file(filename), "input file is not a regular file");

	// extract first byte from infile cipher which must be the mode type used in encryption
	byte cipher_first_byte = static_cast<byte>(Algorithm::Type::Last) + 1;
	if (std::ifstream ifs{ filename, std::ios_base::binary })
	{
		ifs >> cipher_first_byte;
		ifs.close();
	}

	// Check if we detected a valid encryption algorithm mode
	ENIGMA_ASSERT_OR_THROW(ENIGMA_IS_BETWEEN(cipher_first_byte, static_cast<byte>(Algorithm::Type::First), static_cast<byte>(Algorithm::Type::Last)),
		"Could not auto-detect algorithm mode used for encryption, please set it manually with --mode=" + Algorithm::GetSupportedAlgorithmsStr());

	return static_cast<Algorithm::Type>(cipher_first_byte);
}


String Algorithm::AlgoTypeEnumToStr(const Algorithm::Type e) noexcept
{
#define CASE_ENUM(e) case Algorithm::Type::e: return String(#e)
	switch (e)
	{
		CASE_ENUM(AES);
		CASE_ENUM(ChaCha20);
		CASE_ENUM(TripleDES);
		CASE_ENUM(Twofish);
		CASE_ENUM(IDEA);
		default: return "<unknown algorithm>";
	}
#undef CASE_ENUM
}


String Algorithm::GetSupportedAlgorithmsStr() noexcept
{
	String out = "[";
	for (byte i = static_cast<byte>(Algorithm::Type::First); i <= static_cast<byte>(Algorithm::Type::Last); i++)
	{
		out += AlgoTypeEnumToStr(static_cast<Algorithm::Type>(i)) + (i != static_cast<byte>(Algorithm::Type::Last) ? ", " : "");
	}
	out += ']';
	return out;
}

std::vector<std::pair<String, Algorithm::Type>> Algorithm::GetSupportedAlgorithms() noexcept
{
	std::vector<std::pair<String, Algorithm::Type>> algos(static_cast<size_t>(Algorithm::Type::Last));
	for (byte i = static_cast<byte>(Algorithm::Type::First); i <= static_cast<byte>(Algorithm::Type::Last); i++)
	{
		algos[static_cast<size_t>(i) - 1] = std::make_pair(AlgoTypeEnumToStr(static_cast<Algorithm::Type>(i)), static_cast<Algorithm::Type>(i));
	}
	return algos;
}


NS_ENIGMA_END

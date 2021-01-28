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


std::unique_ptr<Algorithm> Algorithm::CreateFromName(const String& mode, const Intent& intent)
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

std::unique_ptr<Algorithm> Algorithm::CreateFromType(const Type& type, const Intent& intent)
{
	String mode = AlgoTypeEnumToStr(type);
	StringUtils::Lower(mode);
	return CreateFromName(mode, intent);
}


String Algorithm::GenerateRandomIV(const size_t& size)
{
	String iv(size, '\000');
	m_auto_seeded_random_pool->GenerateBlock(reinterpret_cast<byte*>(iv.data()), iv.size());
	return iv;
}


String Algorithm::AlgoTypeEnumToStr(const Algorithm::Type& e) noexcept
{
#define CASE_ENUM(e) case Algorithm::Type::e: return #e
	switch (e)
	{
		CASE_ENUM(AES);
		CASE_ENUM(ChaCha20);
		CASE_ENUM(TripleDES);
		CASE_ENUM(Twofish);
		CASE_ENUM(IDEA);
	default: return "Unknown";
	}
#undef CASE_ENUM
}


String Algorithm::GetSupportedAlgorithmsStr() noexcept
{
	String out = "[";
	for (ui8 i = static_cast<ui8>(Algorithm::Type::First); i <= static_cast<ui8>(Algorithm::Type::Last); i++)
	{
		out += AlgoTypeEnumToStr(static_cast<Algorithm::Type>(i)) + (i != ui8(Algorithm::Type::Last) ? ", " : "");
	}
	out += ']';
	return out;
}

std::vector<std::pair<String, Algorithm::Type>> Algorithm::GetSupportedAlgorithms() noexcept
{
	std::vector<std::pair<String, Algorithm::Type>> out(static_cast<size_t>(Algorithm::Type::Last));
	for (ui8 i = static_cast<ui8>(Algorithm::Type::First); i <= static_cast<ui8>(Algorithm::Type::Last); i++)
	{
		out[static_cast<size_t>(i) - 1] = std::make_pair(AlgoTypeEnumToStr(static_cast<Algorithm::Type>(i)), static_cast<Algorithm::Type>(i));
	}
	return out;
}


NS_ENIGMA_END

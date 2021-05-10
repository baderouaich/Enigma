#pragma once
#ifndef ENIGMA_GZIP_COMPRESSOR_H
#define ENIGMA_GZIP_COMPRESSOR_H

#include <Core/Core.hpp>

// Crypto++ GZip Ref: https://www.cryptopp.com/wiki/Gzip && https://www.cryptopp.com/wiki/Gunzip
#include <gzip.h>	// Gzip & Gunzip

NS_ENIGMA_BEGIN

/*
*	.gz Compressor & Decompressor
*/
class ENIGMA_API GZip
{
public:
	enum class DeflateLevel
	{
		/// \brief Minimum deflation level, fastest speed (0)
		MIN = CryptoPP::Gzip::MIN_DEFLATE_LEVEL,
		/// \brief Default deflation level, compromise between speed (6)
		DEFAULT = CryptoPP::Gzip::DEFAULT_DEFLATE_LEVEL,
		/// \brief Minimum deflation level, slowest speed (9)
		MAX = CryptoPP::Gzip::MAX_DEFLATE_LEVEL
	};
public:
	static String Compress(const String& buffer, const DeflateLevel level = DeflateLevel::MAX);
	static String Decompress(const String& buffer);

private:
	inline static std::unique_ptr<CryptoPP::Gzip> m_zipper{ nullptr };
	inline static std::unique_ptr<CryptoPP::Gunzip> m_unzipper{ nullptr };
};

NS_ENIGMA_END

#endif // !ENIGMA_GZIP_COMPRESSOR_H

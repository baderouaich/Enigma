#pragma once
#ifndef ENIGMA_GZIP_COMPRESSOR_H
#define ENIGMA_GZIP_COMPRESSOR_H

#include <Core/Core.hpp>

// Crypto++ GZip Ref: https://www.cryptopp.com/wiki/Gzip && https://www.cryptopp.com/wiki/Gunzip
#include <cryptopp/gzip.h>	// Gzip & Gunzip

NS_ENIGMA_BEGIN

/*
*	.gz Compressor/Decompressor
*/
class ENIGMA_API GZip
{
public:
	static String Compress(const std::string_view& buffer) noexcept(false);
	static String Decompress(const std::string_view& buffer) noexcept(false);

private:
	inline static std::unique_ptr<CryptoPP::Gzip> m_zipper{ nullptr };
	inline static std::unique_ptr<CryptoPP::Gunzip> m_unzipper{ nullptr };
};

NS_ENIGMA_END

#endif // !ENIGMA_GZIP_COMPRESSOR_H

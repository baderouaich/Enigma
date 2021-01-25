#include <pch.hpp>
#include "GZip.hpp"
#include "FileUtils.hpp"

NS_ENIGMA_BEGIN

String GZip::Compress(const std::string_view& buffer) noexcept(false)
{
	String compressed{};
	m_zipper.reset(new CryptoPP::Gzip(new CryptoPP::StringSink(compressed), CryptoPP::Gzip::MAX_DEFLATE_LEVEL));
	m_zipper->Put(reinterpret_cast<const byte*>(buffer.data()), buffer.size());
	m_zipper->MessageEnd();
	return compressed;
}

String GZip::Decompress(const std::string_view& buffer) noexcept(false)
{
	String decompressed{};
	m_unzipper.reset(new CryptoPP::Gunzip(new CryptoPP::StringSink(decompressed)));
	m_unzipper->Put(reinterpret_cast<const byte*>(buffer.data()), buffer.size());
	m_unzipper->MessageEnd();
	return decompressed;

	/*
	String decompressed{};
	m_unzipper.reset(new CryptoPP::Gunzip());
	m_unzipper->Put(reinterpret_cast<const byte*>(buffer.data()), buffer.size());
	m_unzipper->MessageEnd();

	const size_t retrievable_bytes_size = m_unzipper->MaxRetrievable();
	decompressed.resize(retrievable_bytes_size, '\000');
	m_unzipper->Get(reinterpret_cast<byte*>(&decompressed[0]), decompressed.size());
	return decompressed;
	*/
}

NS_ENIGMA_END


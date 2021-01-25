#include <pch.hpp>
#include "GZip.hpp"
#include "FileUtils.hpp"

NS_ENIGMA_BEGIN

String GZip::Compress(const std::string_view& buffer)
{
	String compressed{};
	m_zipper.reset(new CryptoPP::Gzip(new CryptoPP::StringSink(compressed)));
	m_zipper->Put(reinterpret_cast<const byte*>(buffer.data()), buffer.size());
	m_zipper->MessageEnd();
	return compressed;
}

String GZip::Decompress(const std::string_view& buffer)
{
	String decompressed{};
	m_unzipper.reset(new CryptoPP::Gunzip());
	m_unzipper->Put(reinterpret_cast<const byte*>(buffer.data()), buffer.size());
	m_unzipper->MessageEnd();

	const size_t retrievable_bytes_size = m_unzipper->MaxRetrievable();
	decompressed.resize(retrievable_bytes_size, '\000');
	m_unzipper->Get(reinterpret_cast<byte*>(&decompressed[0]), decompressed.size());
	return decompressed;
}

NS_ENIGMA_END


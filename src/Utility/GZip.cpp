#include <pch.hpp>
#include "GZip.hpp"

NS_ENIGMA_BEGIN

#define USE_METHOD_1 false

String GZip::Compress(const String& buffer, const DeflateLevel level)
{
#if USE_METHOD_1 // m1
	String compressed{};
	m_zipper.reset(new CryptoPP::Gzip(nullptr, static_cast<i32>(level)));
	m_zipper->Put(reinterpret_cast<const byte*>(buffer.data()), buffer.size());
	m_zipper->MessageEnd();

	CryptoPP::word64 available = m_zipper->MaxRetrievable();
	if (available)
	{
		compressed.resize(available);
		m_zipper->Get(reinterpret_cast<byte*>(&compressed[0]), compressed.size());
	}
	return compressed;

#else
	// m2
	String compressed{};
	m_zipper.reset(new CryptoPP::Gzip(new CryptoPP::StringSink(compressed), static_cast<ui32>(level)));
	m_zipper->Put(reinterpret_cast<const byte*>(buffer.data()), buffer.size());
	m_zipper->MessageEnd();
	return compressed;
#endif
}

String GZip::Decompress(const String& buffer)
{
#if USE_METHOD_1 // m1
	String decompressed{};
	m_unzipper.reset(new CryptoPP::Gunzip());
	m_unzipper->Put(reinterpret_cast<const byte*>(buffer.data()), buffer.size());
	m_unzipper->MessageEnd();

	CryptoPP::word64 available = m_unzipper->MaxRetrievable();
	if (available)
	{
		decompressed.resize(available);
		m_unzipper->Get(reinterpret_cast<byte*>(&decompressed[0]), decompressed.size());
	}
	return decompressed;
#else
	// m2
	String decompressed{};
	m_unzipper.reset(new CryptoPP::Gunzip(new CryptoPP::StringSink(decompressed)));
	m_unzipper->Put(reinterpret_cast<const byte*>(buffer.data()), buffer.size());
	m_unzipper->MessageEnd();
	return decompressed;
#endif
}

NS_ENIGMA_END


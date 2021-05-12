#pragma once
#ifndef ENIGMA_MODELS_CIPHER_H
#define ENIGMA_MODELS_CIPHER_H

#include <Core/Core.hpp>

NS_ENIGMA_BEGIN

// Cipher Model | table: Cipher
struct Cipher
{
	i64 idc{}; // auto incremental id
	String data{}; // BLOB compressed text or file binary cipher
	i64 ide{}; // foreign key references Encryption(id)

	~Cipher() noexcept
	{
		data.clear();
	}

public:
	friend std::ostream& operator<<(std::ostream& os, const Cipher& c) noexcept
	{
		return os
			<< "Cipher(idc: " << c.idc << ','
			<< "data: " << c.data << ','
			<< "ide: " << c.ide << ')';
	}
	String toString() noexcept
	{
		std::ostringstream oss{};
		oss << *this;
		return oss.str();
	}
};

NS_ENIGMA_END

#endif // !ENIGMA_MODELS_CIPHER_H

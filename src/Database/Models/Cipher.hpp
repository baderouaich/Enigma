#pragma once
#ifndef ENIGMA_MODELS_CIPHER_H
#define ENIGMA_MODELS_CIPHER_H

#include <Core/Core.hpp>

NS_ENIGMA_BEGIN

/**
*	Cipher Model | table: Cipher
*/
struct Cipher
{
	std::int64_t idc{}; /**< auto incremental id */
	std::string data{}; /**< BLOB compressed text or file binary cipher */
	std::int64_t ide{}; /**< foreign key references Encryption(id) */

	~Cipher() = default;

public:	
	constexpr bool operator==(const Cipher& c) const noexcept { return idc == c.idc; }
	friend std::ostream& operator<<(std::ostream& os, const Cipher& c) noexcept
	{
		return os
			<< "Cipher(idc: " << c.idc << ','
			<< "data: " << c.data << ','
			<< "ide: " << c.ide << ')';
	}
	std::string toString() const noexcept
	{
		std::ostringstream oss{};
		oss << *this;
		return oss.str();
	}
};

NS_ENIGMA_END

#endif // !ENIGMA_MODELS_CIPHER_H

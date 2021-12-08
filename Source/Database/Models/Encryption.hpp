#pragma once
#ifndef ENIGMA_MODELS_ENCRYPTION_H
#define ENIGMA_MODELS_ENCRYPTION_H

#include <Core/Core.hpp>
#include "Cipher.hpp"

#include <sstream>	//	std::ostringstream

NS_ENIGMA_BEGIN

/**
*	Encryption Model | Table: Encryption
*/
struct Encryption
{
	i64 ide{};  /**< auto incremental id in range [0, SIZE_MAX] */
	String title{}; /**< encryption title, e.g: "this is my Email password" */
	Cipher cipher{}; /**< encryption encrypted data, aka cipher. from another table for performance reasons */
	String date_time{}; /**< date & time saved by sqlite3 */
	i64 size{}; /**< size of compressed cipher in bytes */
	bool is_file{}; /**< whether its a text or file encryption */
	String file_ext{}; /**< file extension to remember file type on decryption */

	~Encryption() noexcept
	{
		title.clear();
		date_time.clear();
	}

public:
	constexpr bool operator==(const Encryption& e) const noexcept { return ide == e.ide; }
	friend std::ostream& operator<<(std::ostream& os, const Encryption& e) noexcept 
	{
		return os
			<< "Encryption(ide: " << e.ide << ','
			<< "title: " << e.title << ','
			<< "cipher: " << e.cipher << ','
			<< "date_time: " << e.date_time << ','
			<< "is_file: "<< std::boolalpha << e.is_file << ',' 
			<< "file_ext:" << e.file_ext << ')';
		
	}
	String toString() noexcept
	{
		std::ostringstream oss{};
		oss << *this;
		return oss.str();
	}
};

NS_ENIGMA_END

#endif // !ENIGMA_MODELS_ENCRYPTION_H

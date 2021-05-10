#pragma once
#ifndef ENIGMA_MODELS_ENCRYPTION_H
#define ENIGMA_MODELS_ENCRYPTION_H

#include <Core/Core.hpp>
#include "Cipher.hpp"

#include <sstream>	//	std::ostringstream

NS_ENIGMA_BEGIN
// Encryption Model | table: Encryption
struct Encryption
{
	i64 ide{}; // auto incremental id 0 -> SIZE_MAX
	String title{}; // E.G: "this is my gmail password"
	Cipher cipher{}; 
	String date_time{}; // date & time saved by sqlite3
	i64 size{}; // size of compressed cipher in bytes
	bool is_file{}; // whether its a text or file encryption


public:
	friend std::ostream& operator<<(std::ostream& os, const Encryption& e) noexcept 
	{
		return os
			<< "Encryption(ide: " << e.ide << ','
			<< "title: " << e.title << ','
			<< "cipher: " << e.cipher << ','
			<< "date_time: " << e.date_time << ','
			<< "is_file: "<< std::boolalpha << e.is_file << ')';
		
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

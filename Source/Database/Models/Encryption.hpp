#pragma once
#ifndef ENIGMA_MODELS_ENCRYPTION_H
#define ENIGMA_MODELS_ENCRYPTION_H

#include <Core/Core.hpp>

NS_ENIGMA_BEGIN
struct Encryption
{
	size_t id;
	String title; // E.G: this is my facebook password
	String cipher; // BLOB
	time_t date_time; // date time saved
	bool compressed; // always
};
NS_ENIGMA_END

#endif // !ENIGMA_MODELS_ENCRYPTION_H

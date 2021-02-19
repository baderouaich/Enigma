#pragma once
#ifndef ENIGMA_RANDOM_H
#define ENIGMA_RANDOM_H

#include <Core/Core.hpp>
#include <random>

NS_ENIGMA_BEGIN
class ENIGMA_API Random
{
public:
	/*
	*	Returns a Random Real between min and max
	*/
	template<typename T>
	static typename std::enable_if<std::is_floating_point<T>::value, T>::type
	Real(T min, T max) noexcept
	{
		ENIGMA_ASSERT(min < max, "min is >= max");
		std::uniform_real_distribution<T> dist(min, max);
		auto& mt = GetEngine();
		return dist(mt);
	}

	/*
	*	Returns a Random Integer between min and max
	*/
	template<typename T>
	static typename std::enable_if<std::is_integral<T>::value && !std::is_same<T, bool>::value, T>::type
	Int(T min, T max) noexcept
	{
		ENIGMA_ASSERT(min < max, "min is >= max");
		std::uniform_int_distribution<T> dist(min, max);
		auto& mt = GetEngine();
		return dist(mt);
	}

	/*
    *	Returns a Random bool value, either 'true' or 'false' 
	*/
	static bool Bool(f64 chance = 0.5 /*50% 50% chance*/) noexcept
	{
		std::bernoulli_distribution dist(chance);
		auto& mt = GetEngine();
		return !!dist(mt);
	}
	
private:
	static std::mt19937& GetEngine();
};
NS_ENIGMA_END

#endif // !ENIGMA_RANDOM_H

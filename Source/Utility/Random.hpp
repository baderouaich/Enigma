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
	Real(const T min, const T max) noexcept
	{
		ENIGMA_ASSERT(min < max, "min is >= max");
		std::uniform_real_distribution<T> dist(min, max);
		return dist(m_engine);
	}

	/*
	*	Returns a Random Integer between min and max
	*/
	template<typename T>
	static typename std::enable_if<std::is_integral<T>::value && !std::is_same<T, bool>::value, T>::type
	Int(const T min, const T max) noexcept
	{
		ENIGMA_ASSERT(min < max, "min is >= max");
		std::uniform_int_distribution<T> dist(min, max);
		return dist(m_engine);
	}

	/*
	*	Returns a Random bool value, either 'true' or 'false'
	*	@param p: Probability of true (50% by default)
	*/
	static bool Bool(const f64 p = 0.5) noexcept
	{
		std::bernoulli_distribution dist(p);
		return !!dist(m_engine);
	}

	/*
	*	Returns a Random String(length) consists of alphanumeric characters
	*/
	static String Str(const size_t length) noexcept
	{
		String str(length, '\000');
		for (char& c : str)
		{
			const i16 i = Int(static_cast<i16>(1), static_cast<i16>(3));
			switch (i)
			{
			case 1: // a-z
				c = Int(static_cast<i16>('a'), static_cast<i16>('z'));
				break;
			case 2: // A-Z
				c = Int(static_cast<i16>('A'), static_cast<i16>('Z'));
				break;
			case 3: // 0-9
				c = Int(static_cast<i16>('0'), static_cast<i16>('9')); // ascii 0 = 48 | 9 = 57
				break;
			}
		}
		return str;
	}

private:
	inline static std::random_device m_seed{};
	inline static std::default_random_engine m_engine{ m_seed() };
};
NS_ENIGMA_END

#endif // !ENIGMA_RANDOM_H

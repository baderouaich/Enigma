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
	*	@brief Generates a random real between a range
	*	@param min: minimum value of range (included)
	*	@param max: maximum value of range (included)
	*	@return Random real between min and max
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
	*	@brief Generates a random integer between a range
	*	@param min: minimum value of range (included)
	*	@param max: maximum value of range (included)
	*	@return Random integer between min and max
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
	*	@brief Generates a random boolean value
	*	@param p: Probability of true (50% by default)
	*	@return Returns a Random bool value, either 'true' or 'false'
	*/
	static bool Bool(const f64 p = 0.5) noexcept
	{
		std::bernoulli_distribution dist(p);
		return !!dist(m_engine);
	}

	/*
	*	@brief Generates a random string with length
	*	@param length: length of the generated random string
	*	@return Returns a Random String with specified length consists of alphanumeric characters and special characters
	*/
	static String Str(const size_t length) noexcept
	{
		String str(length, '\000');
		for (char& c : str)
		{
			const i16 i = Int(static_cast<i16>(1), static_cast<i16>(4));
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
			case 4: // special characters
				using Constants::Algorithm::SPECIAL_CHARACTERS;
				c = SPECIAL_CHARACTERS[Int(static_cast<size_t>(0), ENIGMA_ARRAY_SIZE(SPECIAL_CHARACTERS))];
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

#pragma once
#ifndef ENIGMA_RANDOM_H
#define ENIGMA_RANDOM_H

#include <Core/Core.hpp>
#include <random>
#include <algorithm>
//#include <execution> // std::for_each(std::execution::par, ...)

NS_ENIGMA_BEGIN
class ENIGMA_API Random final
{
	ENIGMA_STATIC_CLASS(Random);
public:
	/**
	*	@brief Generates a random real between a range
	*	@param min: minimum value of range (included in range)
	*	@param max: maximum value of range (included in range)
	*	@return Random real between min and max
	*/
	template<typename T>
	static typename std::enable_if<std::is_floating_point<T>::value, T>::type
	Real(const T min, const T max)
	{
		ENIGMA_ASSERT(min < max, "min random value should be less than max value");
		std::uniform_real_distribution<T> dist(min, max);
		return dist(m_engine);
	}

	/**
	*	@brief Generates a random integer between a range
	*	@param min: minimum value of range (included in range)
	*	@param max: maximum value of range (included in range)
	*	@return Random integer between min and max
	*/
	template<typename T>
	static typename std::enable_if<std::is_integral<T>::value && !std::is_same<T, bool>::value, T>::type
	Int(const T min, const T max)
	{
		ENIGMA_ASSERT(min < max, "min random value should be less than max value");
		std::uniform_int_distribution<T> dist(min, max);
		return dist(m_engine);
	}

	/**
	*	@brief Generates a random boolean value
	*	@param p: Probability of true (50% by default)
	*	@return Returns a Random bool value, either 'true' or 'false'
	*/
	static bool Bool(const f64 p = 0.5)
	{
		std::bernoulli_distribution dist(p);
		return !!dist(m_engine);
	}
	

	/** @brief Generates a random string
	*	@param length: length of the random string to generate
	*	@param parallel: if true, a random string will be generated as fast as possible using multiple threads (usefull when string is too large)
	*	@returns a random String with specified length consists of alphanumeric and special characters
	*/
	static String Str(const size_t length, const bool parallel = false) noexcept
	{
		String out(length, '\000');
		const std::string_view special_characters = Constants::Algorithm::SPECIAL_CHARACTERS;
#if 0
		if (parallel)
		{
			std::mutex mtx{};
			std::for_each(std::execution::par, out.begin(), out.end(), [&special_characters, &mtx](char& c)
				{
					std::scoped_lock<std::mutex> guard{ mtx }; // guard scope, note that most of functions bellow us assertions, without guarding can result on bad rare behavior where two threads assert at the same time. its just nasty, use lock guards.
					if (const ui16 r = Random::Int<ui16>(0, 3); r == 0)
						c = static_cast<char>(Random::Int(ui16('a'), ui16('z'))); // alpha lower
					else if (r == 1) // alpha upper
						c = static_cast<char>(Random::Int(ui16('A'), ui16('Z')));
					else if (r == 2) // digits
						c = static_cast<char>(Random::Int(ui16('0'), ui16('9')));
					else if (r == 3) // special characters
						c = special_characters[Random::Int<size_t>(0, special_characters.size() - 1)];
				});
		}
		else
#endif
		{
			for (char& c : out)
			{
				if (const ui16 r = Random::Int<ui16>(0, 3); r == 0)
					c = static_cast<char>(Random::Int(ui16('a'), ui16('z'))); // alpha lower
				else if (r == 1) // alpha upper
					c = static_cast<char>(Random::Int(ui16('A'), ui16('Z')));
				else if (r == 2) // digits
					c = static_cast<char>(Random::Int(ui16('0'), ui16('9')));
				else if (r == 3) // special characters
					c = special_characters[Random::Int<size_t>(0, special_characters.size() - 1)];
			}
		}
		return out;
}


	/** Reseeds the random engine with a new value */
	static void Reseed()
	{
		m_engine.seed(m_seed());
	}

private:
	inline static std::random_device m_seed{};
	inline static std::default_random_engine m_engine{ m_seed() };
};
NS_ENIGMA_END

#endif // !ENIGMA_RANDOM_H

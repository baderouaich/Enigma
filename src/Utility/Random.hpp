#pragma once
#ifndef ENIGMA_RANDOM_H
#define ENIGMA_RANDOM_H

#include <Core/Core.hpp>
#include <algorithm>
#include <random>
//#include <execution> // std::for_each(std::execution::par, ...)

NS_ENIGMA_BEGIN
class Random final {
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
    Real(const T min, const T max) {
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
    Int(const T min, const T max) {
      ENIGMA_ASSERT(min < max, "min random value should be less than max value");
      std::uniform_int_distribution<T> dist(min, max);
      return dist(m_engine);
    }

    /**
	*	@brief Generates a random boolean value
	*	@param p: Probability of true (50% by default)
	*	@return Returns a Random bool value, either 'true' or 'false'
	*/
    static bool Bool(const double p = 0.5) {
      std::bernoulli_distribution dist(p);
      return !!dist(m_engine);
    }


    /** @brief Generates a random string
    *	@param length: length of the random string to generate
    *	@returns a random std::string a-zA-Z0-9
    */
    static std::string Str(const std::size_t length) noexcept {
      static std::random_device seed{};
      static std::default_random_engine engine{seed()};
      static std::uniform_int_distribution<short> choice(0, 2);
      static std::uniform_int_distribution<int> lowercaseAlpha('a', 'z');
      static std::uniform_int_distribution<int> uppercaseAlpha('A', 'Z');
      static std::uniform_int_distribution<int> digits('0', '9');

      std::string str(length, '\000');
      for (char& c: str) {
        switch (choice(engine)) {
          case 0: // a-z
            c = lowercaseAlpha(engine);
            break;
          case 1: // A-Z
            c = uppercaseAlpha(engine);
            break;
          case 2: // 0-9
            c = digits(engine);
            break;
          default:
            break;
        }
      }
      return str;
    }


    /** Reseeds the random engine with a new value */
    static void Reseed() {
      m_engine.seed(m_seed());
    }

  private:
    inline static std::random_device m_seed{};
    inline static std::default_random_engine m_engine{m_seed()};
};
NS_ENIGMA_END

#endif // !ENIGMA_RANDOM_H

#include <pch.hpp>
#include "Random.hpp"

NS_ENIGMA_BEGIN
std::mt19937& Random::GetEngine()
{
	static std::random_device seed;
	static std::mt19937 engine(seed());
	return engine;
}
NS_ENIGMA_END


#pragma once
#ifndef ENIGMA_MATH_UTILS_H
#define ENIGMA_MATH_UTILS_H

#include <Core/Core.hpp>
#include <cmath>	// std::log, std::floor, std::round...

NS_ENIGMA_BEGIN

class ENIGMA_API MathUtils
{
public:
	/*
	*	Maps value from a range to another
	*	@param value: the incoming value to be converted
	*	@param start1: lower bound of the value's current range
	*	@param stop1: upper bound of the value's current range
	*	@param start2: lower bound of the value's target range
	*	@param stop2: upper bound of the value's target range
	*/
	template<typename T>
	[[no_discard]] static constexpr const T Map(const T& value, const T& start1, const T& stop1, const T& start2, const T& stop2) noexcept
	{
		return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
	}

};


NS_ENIGMA_END

#endif // !ENIGMA_MATH_UTILS_H

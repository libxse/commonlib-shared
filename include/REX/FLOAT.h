#pragma once

#include "REX/BASE.h"

namespace REX::FLT
{
	constexpr float EPSILON = std::numeric_limits<float>::epsilon();

	inline bool APPROXIMATELY_EQUAL(const float a, const float b)
	{
		return std::fabs(a - b) <= ((std::fabs(a) < std::fabs(b) ? std::fabs(b) : std::fabs(a)) * EPSILON);
	}

	inline bool ESSENTIALLY_EQUAL(const float a, const float b)
	{
		return std::fabs(a - b) <= ((std::fabs(a) > std::fabs(b) ? std::fabs(b) : std::fabs(a)) * EPSILON);
	}

	inline bool DEFINITELY_GREATER(const float a, const float b)
	{
		return (a - b) > ((std::fabs(a) < std::fabs(b) ? std::fabs(b) : std::fabs(a)) * EPSILON);
	}

	inline bool DEFINITELY_LESS(const float a, const float b)
	{
		return (b - a) > ((std::fabs(a) < std::fabs(b) ? std::fabs(b) : std::fabs(a)) * EPSILON);
	}
}

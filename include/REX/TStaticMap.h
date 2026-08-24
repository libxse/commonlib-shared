#pragma once

#include "REX/BASE.h"

namespace REX
{
	template <typename T, typename U, std::size_t S>
	struct TStaticMap
	{
		[[nodiscard]] constexpr T at(const U& a_key) const
		{
			const auto iter = std::find_if(data.begin(), data.end(),
				[&a_key](const auto& v) { return v.first == a_key; });
			if (iter == data.end()) {
				throw std::range_error("key not found");
			}
			return iter->second;
		}

		[[nodiscard]] constexpr U key(const T& a_value) const
		{
			const auto iter = std::find_if(data.begin(), data.end(),
				[&a_value](const auto& v) { return v.second == a_value; });
			if (iter == data.end()) {
				throw std::range_error("value not found");
			}
			return iter->first;
		}

		[[nodiscard]] constexpr T operator[](const U& a_key) const
		{
			return at(a_key);
		}

		std::array<std::pair<T, U>, S> data;
	};
}

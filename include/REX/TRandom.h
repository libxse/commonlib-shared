#pragma once

#ifdef COMMONLIB_OPTION_RANDOM

#	include "REX/BASE.h"

namespace REX
{
	template <typename T>
		requires std::is_arithmetic_v<T>
	class TRandom
	{
	public:
		TRandom();
		TRandom(std::uint32_t a_seed);
		TRandom(std::uint64_t a_seed);

		static constexpr T min() { return std::numeric_limits<T>::min(); }
		static constexpr T max() { return std::numeric_limits<T>::max(); }

		T Generate(T a_min = min(), T a_max = max());

	protected:
		std::byte m_rng[32];
	};

	template <typename T>
		requires std::is_integral_v<T>
	class TRandomDistribution
	{
	public:
		TRandomDistribution() = delete;
		TRandomDistribution(std::vector<std::uint32_t>& a_weights);
		TRandomDistribution(std::uint32_t a_seed, std::vector<std::uint32_t>& a_weights);
		TRandomDistribution(std::uint64_t a_seed, std::vector<std::uint32_t>& a_weights);

		static constexpr T min() { return std::numeric_limits<T>::min(); }
		static constexpr T max() { return std::numeric_limits<T>::max(); }

		T Generate();

	private:
		std::byte                     m_rng[32];
		std::discrete_distribution<T> m_dist;
	};
}

namespace REX::RNG
{
	using F32 = TRandom<float>;
	using F64 = TRandom<double>;
	using I32 = TRandom<std::int32_t>;
	using I64 = TRandom<std::int64_t>;
	using U32 = TRandom<std::uint32_t>;
	using U64 = TRandom<std::uint64_t>;
	using I32D = TRandomDistribution<std::int32_t>;
	using I64D = TRandomDistribution<std::int64_t>;
	using U32D = TRandomDistribution<std::uint32_t>;
	using U64D = TRandomDistribution<std::uint64_t>;
}

#endif

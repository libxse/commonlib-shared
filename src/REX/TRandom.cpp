#ifdef COMMONLIB_OPTION_RANDOM

#	include "REX/TRandom.h"

#	include <XoshiroCpp.hpp>

#	define REX_DEFINE_RANDOM_FUNCTIONS(KIND, T)                                                             \
		template <>                                                                                          \
		TRandom<T>::TRandom()                                                                                \
		{                                                                                                    \
			XoshiroCpp::Xoshiro256StarStar rng(std::chrono::steady_clock::now().time_since_epoch().count()); \
			std::memcpy(&m_rng, &rng, 32);                                                                   \
		}                                                                                                    \
		template <>                                                                                          \
		TRandom<T>::TRandom(std::uint32_t a_seed)                                                            \
		{                                                                                                    \
			XoshiroCpp::Xoshiro256StarStar rng(a_seed);                                                      \
			std::memcpy(&m_rng, &rng, 32);                                                                   \
		}                                                                                                    \
		template <>                                                                                          \
		TRandom<T>::TRandom(std::uint64_t a_seed)                                                            \
		{                                                                                                    \
			XoshiroCpp::Xoshiro256StarStar rng(a_seed);                                                      \
			std::memcpy(&m_rng, &rng, 32);                                                                   \
		}                                                                                                    \
		template <>                                                                                          \
		T TRandom<T>::Generate(T a_min, T a_max)                                                             \
		{                                                                                                    \
			std::uniform_##KIND##_distribution<T> dist(a_min, a_max);                                        \
			return dist(*reinterpret_cast<XoshiroCpp::Xoshiro256StarStar*>(m_rng));                          \
		}

#	define REX_DEFINE_RANDOM_DIST_FUNCTIONS(T)                                                                    \
		template <>                                                                                                \
		TRandomDistribution<T>::TRandomDistribution(std::vector<std::uint32_t>& a_weights) :                       \
			m_dist(a_weights.begin(), a_weights.end())                                                             \
		{                                                                                                          \
			XoshiroCpp::Xoshiro256StarStar rng(std::chrono::steady_clock::now().time_since_epoch().count());       \
			std::memcpy(&m_rng, &rng, sizeof(m_rng));                                                              \
		}                                                                                                          \
		template <>                                                                                                \
		TRandomDistribution<T>::TRandomDistribution(std::uint32_t a_seed, std::vector<std::uint32_t>& a_weights) : \
			m_dist(a_weights.begin(), a_weights.end())                                                             \
		{                                                                                                          \
			XoshiroCpp::Xoshiro256StarStar rng(a_seed);                                                            \
			std::memcpy(&m_rng, &rng, sizeof(m_rng));                                                              \
		}                                                                                                          \
		template <>                                                                                                \
		TRandomDistribution<T>::TRandomDistribution(std::uint64_t a_seed, std::vector<std::uint32_t>& a_weights) : \
			m_dist(a_weights.begin(), a_weights.end())                                                             \
		{                                                                                                          \
			XoshiroCpp::Xoshiro256StarStar rng(a_seed);                                                            \
			std::memcpy(&m_rng, &rng, sizeof(m_rng));                                                              \
		}                                                                                                          \
		template <>                                                                                                \
		T TRandomDistribution<T>::Generate()                                                                       \
		{                                                                                                          \
			return m_dist(*reinterpret_cast<XoshiroCpp::Xoshiro256StarStar*>(m_rng));                              \
		}

namespace REX
{
	REX_DEFINE_RANDOM_FUNCTIONS(real, float);
	REX_DEFINE_RANDOM_FUNCTIONS(real, double);
	REX_DEFINE_RANDOM_FUNCTIONS(int, std::int32_t);
	REX_DEFINE_RANDOM_FUNCTIONS(int, std::int64_t);
	REX_DEFINE_RANDOM_FUNCTIONS(int, std::uint32_t);
	REX_DEFINE_RANDOM_FUNCTIONS(int, std::uint64_t);
	REX_DEFINE_RANDOM_DIST_FUNCTIONS(std::int32_t);
	REX_DEFINE_RANDOM_DIST_FUNCTIONS(std::int64_t);
	REX_DEFINE_RANDOM_DIST_FUNCTIONS(std::uint32_t);
	REX_DEFINE_RANDOM_DIST_FUNCTIONS(std::uint64_t);
}

#	undef XOSHIROCPP_NODISCARD_CXX20
#	undef REX_DEFINE_RANDOM_FUNCTIONS
#	undef REX_DEFINE_RANDOM_DIST_FUNCTIONS

#endif

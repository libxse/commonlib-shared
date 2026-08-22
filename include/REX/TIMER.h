#pragma once

#include "REX/BASE.h"

namespace REX
{
	class TIMER
	{
	public:
		void Start()
		{
			m_start = std::chrono::steady_clock::now();
		}

		void Stop()
		{
			m_end = std::chrono::steady_clock::now();
			m_duration = m_end - m_start;
		}

		template <class T>
		[[nodiscard]] std::uint64_t GetDuration() const
		{
			return std::chrono::duration_cast<T>(m_duration).count();
		}

		[[nodiscard]] std::uint64_t GetDuration_us() const { return GetDuration<std::chrono::microseconds>(); }
		[[nodiscard]] std::uint64_t GetDuration_ms() const { return GetDuration<std::chrono::milliseconds>(); }
		[[nodiscard]] std::uint64_t GetDuration_s() const { return GetDuration<std::chrono::seconds>(); }
		[[nodiscard]] std::uint64_t GetDuration_m() const { return GetDuration<std::chrono::minutes>(); }
		[[nodiscard]] std::uint64_t GetDuration_h() const { return GetDuration<std::chrono::hours>(); }

		[[nodiscard]] std::string GetDurationString_us() const { return std::format("{} us"sv, GetDuration_us()); }
		[[nodiscard]] std::string GetDurationString_ms() const { return std::format("{} ms"sv, GetDuration_ms()); }
		[[nodiscard]] std::string GetDurationString_s() const { return std::format("{} s"sv, GetDuration_s()); }
		[[nodiscard]] std::string GetDurationString_m() const { return std::format("{} m"sv, GetDuration_m()); }
		[[nodiscard]] std::string GetDurationString_h() const { return std::format("{} h"sv, GetDuration_h()); }

		[[nodiscard]] std::string GetDurationStringMin() const
		{
			if (m_duration < std::chrono::milliseconds(1)) {
				return GetDurationString_us();
			} else if (m_duration < std::chrono::seconds(1)) {
				return GetDurationString_ms();
			} else if (m_duration < std::chrono::minutes(1)) {
				return GetDurationString_s();
			} else if (m_duration < std::chrono::hours(1)) {
				return GetDurationString_m();
			} else if (m_duration < std::chrono::days(1)) {
				return GetDurationString_h();
			} else [[unlikely]] {
				return "> 1 day"s;
			}
		}

	private:
		std::chrono::steady_clock::time_point m_start;
		std::chrono::steady_clock::time_point m_end;
		std::chrono::nanoseconds              m_duration;
	};
}

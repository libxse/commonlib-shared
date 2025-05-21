#pragma once

#include "REX/BASE.h"

#include "REL/Segment.h"
#include "REL/Version.h"

#include "REX/REX/Singleton.h"

namespace REL
{
	class Module :
		public REX::Singleton<Module>
	{
	public:
		Module();

		[[nodiscard]] constexpr std::uintptr_t base() const noexcept { return _base; }
		[[nodiscard]] std::wstring_view        filename() const noexcept { return _filename; }
		[[nodiscard]] constexpr Segment        segment(Segment::Name a_segment) const noexcept { return _segments[a_segment]; }

		[[nodiscard]] constexpr Version version() const noexcept
		{
			return _version;
		}

		constexpr void version(Version a_version) noexcept
		{
			_version = a_version;
		}

		[[nodiscard]] void* pointer() const noexcept { return reinterpret_cast<void*>(base()); }

		template <class T>
		[[nodiscard]] T* pointer() const noexcept
		{
			return static_cast<T*>(pointer());
		}

	private:
		void load_segments();

		static constexpr std::array SEGMENTS{
			".text"sv,
			".interpr"sv,
			".idata"sv,
			".rdata"sv,
			".data"sv,
			".pdata"sv,
			".tls"sv
		};

		static inline std::uintptr_t _natvis{ 0 };

		std::wstring                        _filename;
		std::array<Segment, Segment::total> _segments;
		Version                             _version;
		std::uintptr_t                      _base{ 0 };
	};
}

#include "REL/Module.h"

#include "REX/REX/CAST.h"
#include "REX/REX/LOG.h"
#include "REX/W32/KERNEL32.h"

namespace REL
{
	Module::Module() :
		_base(reinterpret_cast<std::uintptr_t>(REX::W32::GetModuleHandleA(nullptr)))
	{
		_natvis = _base;

		char path[REX::W32::MAX_PATH];
		if (!REX::W32::GetModuleFileNameA(reinterpret_cast<REX::W32::HMODULE>(_base), path, REX::W32::MAX_PATH))
			REX::FAIL("failed to obtain file name");

		_filename = std::filesystem::path(path).wstring();

		if (const auto version = GetFileVersion(_filename)) {
			_version = *version;
		} else {
			REX::FAIL("failed to obtain file version");
		}

		load_segments();
	}

	void Module::load_segments()
	{
		const auto dosHeader = reinterpret_cast<const REX::W32::IMAGE_DOS_HEADER*>(_base);
		const auto ntHeader = REX::ADJUST_POINTER<REX::W32::IMAGE_NT_HEADERS64>(dosHeader, dosHeader->lfanew);
		const auto sections = REX::W32::IMAGE_FIRST_SECTION(ntHeader);
		const auto size = std::min<std::size_t>(ntHeader->fileHeader.sectionCount, _segments.size());
		for (std::size_t i = 0; i < size; ++i) {
			const auto& section = sections[i];
			const auto  it = std::find_if(SEGMENTS.begin(), SEGMENTS.end(), [&](auto&& a_elem) {
                constexpr auto size = std::extent_v<decltype(section.name)>;
                const auto     len = std::min(a_elem.size(), size);
                return std::memcmp(a_elem.data(), section.name, len) == 0;
            });
			if (it != SEGMENTS.end()) {
				const auto idx = static_cast<std::size_t>(std::distance(SEGMENTS.begin(), it));
				_segments[idx] = Segment{ _base, _base + section.virtualAddress, section.virtualSize };
			}
		}
	}
}

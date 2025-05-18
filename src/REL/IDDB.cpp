#include "REL/IDDB.h"

namespace REL
{
	std::size_t IDDB::id2offset(std::uint64_t a_id) const
	{
		if (_id2offset.empty()) {
			stl::report_and_fail("No Address Library has been loaded!"sv);
		}

		const mapping_t elem{ a_id, 0 };
		const auto      it = std::lower_bound(
            _id2offset.begin(),
            _id2offset.end(),
            elem,
            [](auto&& a_lhs, auto&& a_rhs) {
                return a_lhs.id < a_rhs.id;
            });

		if (it == _id2offset.end()) {
			const auto mod = Module::GetSingleton();
			const auto version = mod->version();
			const auto str = std::format(
				"Failed to find offset for Address Library ID!\n"
				"Invalid ID: {}\n"
				"Game Version: {}"sv,
				a_id, version.string());
			stl::report_and_fail(str);
		}

		return static_cast<std::size_t>(it->offset);
	}
}

#ifndef XSE_MMIO_ADDRESSLIB
namespace REL
{
	bool IDDB::memory_map::open(stl::zwstring a_name, std::size_t a_size)
	{
		close();

		REX::W32::ULARGE_INTEGER bytes;
		bytes.value = a_size;

		_mapping = REX::W32::OpenFileMappingW(
			REX::W32::FILE_MAP_READ | REX::W32::FILE_MAP_WRITE,
			false,
			a_name.data());

		if (!_mapping) {
			close();
			return false;
		}

		_view = REX::W32::MapViewOfFile(
			_mapping,
			REX::W32::FILE_MAP_READ | REX::W32::FILE_MAP_WRITE,
			0,
			0,
			bytes.value);

		if (!_view) {
			close();
			return false;
		}

		return true;
	}

	bool IDDB::memory_map::create(stl::zwstring a_name, std::size_t a_size)
	{
		close();

		REX::W32::ULARGE_INTEGER bytes;
		bytes.value = a_size;

		_mapping = REX::W32::OpenFileMappingW(
			REX::W32::FILE_MAP_READ | REX::W32::FILE_MAP_WRITE,
			false,
			a_name.data());

		if (!_mapping) {
			_mapping = REX::W32::CreateFileMappingW(
				REX::W32::INVALID_HANDLE_VALUE,
				nullptr,
				REX::W32::PAGE_READWRITE,
				bytes.hi,
				bytes.lo,
				a_name.data());

			if (!_mapping) {
				return false;
			}
		}

		_view = REX::W32::MapViewOfFile(
			_mapping,
			REX::W32::FILE_MAP_READ | REX::W32::FILE_MAP_WRITE,
			0,
			0,
			bytes.value);

		if (!_view) {
			return false;
		}

		return true;
	}

	void IDDB::memory_map::close()
	{
		if (_view) {
			REX::W32::UnmapViewOfFile(static_cast<const void*>(_view));
			_view = nullptr;
		}

		if (_mapping) {
			REX::W32::CloseHandle(_mapping);
			_mapping = nullptr;
		}
	}

	void IDDB::header_t::read(istream_t& a_in, const DatabaseVersion a_version)
	{
		const auto format = a_in.readout<std::uint32_t>();
		if (format != a_version) {
			const auto str = std::format(
				"Unsupported Address Library format!\n"
				"Expected version: {}\n"
				"Actual version: {}"sv,
				std::to_underlying(a_version), format);
			stl::report_and_fail(str);
		}

		std::uint32_t version[4]{};
		a_in.readin(version);
		for (std::size_t i = 0; i < 4; i++) {
			_version[i] = static_cast<std::uint16_t>(version[i]);
		}

		const auto nameLen = a_in.readout<std::uint32_t>();
		for (std::uint32_t i = 0; i < nameLen; i++) {
			a_in.readin(_name[i]);
		}
		_name[nameLen] = '\0';

		a_in.readin(_pointerSize);
		a_in.readin(_addressCount);
	}

	void IDDB::load(std::wstring_view a_filename, const DatabaseVersion a_version)
	{
		assert(a_filename.size() > 0);

		const auto mod = Module::GetSingleton();
		const auto version = mod->version().wstring(L"-"sv);
		const auto filename = std::vformat(a_filename, std::make_wformat_args(version));

		try {
			istream_t in(filename.data(), std::ios::in | std::ios::binary);
			header_t  header;

			header.read(in, a_version);
			if (header.version() != mod->version()) {
				const auto str = std::format(
					"Address Library version mismatch!\n"
					"Expected Version: {}\n"
					"Actual Version: {}"sv,
					mod->version().string(), header.version().string());
				stl::report_and_fail(str);
			}

			const auto byteSize = static_cast<std::size_t>(header.address_count()) * sizeof(mapping_t);
			if (_mmap.open(L"XSEOffsets"sv, byteSize)) {
				_id2offset = { static_cast<mapping_t*>(_mmap.data()), header.address_count() };
			} else if (_mmap.create(L"XSEOffsets"sv, byteSize)) {
				_id2offset = { static_cast<mapping_t*>(_mmap.data()), header.address_count() };
				unpack_file(in, header);
				std::sort(
					_id2offset.begin(),
					_id2offset.end(),
					[](auto&& a_lhs, auto&& a_rhs) {
						return a_lhs.id < a_rhs.id;
					});
			} else {
				stl::report_and_fail("Failed to create shared mapping!"sv);
			}
		} catch (const std::system_error&) {
			const auto str = std::format(
				L"Failed to open Address Library file!\nPath: {}"sv,
				filename);
			stl::report_and_fail(str);
		}
	}

	void IDDB::unpack_file(istream_t& a_in, const header_t& a_header)
	{
		std::uint8_t  type = 0;
		std::uint64_t id = 0;
		std::uint64_t offset = 0;
		std::uint64_t prevID = 0;
		std::uint64_t prevOffset = 0;
		for (auto& mapping : _id2offset) {
			a_in.readin(type);
			const auto lo = static_cast<std::uint8_t>(type & 0xF);
			const auto hi = static_cast<std::uint8_t>(type >> 4);

			switch (lo) {
				case 0:
					a_in.readin(id);
					break;
				case 1:
					id = prevID + 1;
					break;
				case 2:
					id = prevID + a_in.readout<std::uint8_t>();
					break;
				case 3:
					id = prevID - a_in.readout<std::uint8_t>();
					break;
				case 4:
					id = prevID + a_in.readout<std::uint16_t>();
					break;
				case 5:
					id = prevID - a_in.readout<std::uint16_t>();
					break;
				case 6:
					id = a_in.readout<std::uint16_t>();
					break;
				case 7:
					id = a_in.readout<std::uint32_t>();
					break;
				default:
					stl::report_and_fail("Unhandled type while loading Address Library!"sv);
			}

			const std::uint64_t tmp = (hi & 8) != 0 ? (prevOffset / a_header.pointer_size()) : prevOffset;

			switch (hi & 7) {
				case 0:
					a_in.readin(offset);
					break;
				case 1:
					offset = tmp + 1;
					break;
				case 2:
					offset = tmp + a_in.readout<std::uint8_t>();
					break;
				case 3:
					offset = tmp - a_in.readout<std::uint8_t>();
					break;
				case 4:
					offset = tmp + a_in.readout<std::uint16_t>();
					break;
				case 5:
					offset = tmp - a_in.readout<std::uint16_t>();
					break;
				case 6:
					offset = a_in.readout<std::uint16_t>();
					break;
				case 7:
					offset = a_in.readout<std::uint32_t>();
					break;
				default:
					stl::report_and_fail("Unhandled type while loading Address Library!"sv);
			}

			if ((hi & 8) != 0) {
				offset *= a_header.pointer_size();
			}

			mapping = { id, offset };

			prevOffset = offset;
			prevID = id;
		}
	}
}
#else
namespace REL
{
	void IDDB::load(std::string_view a_filename)
	{
		assert(a_filename.size() > 0);

		const auto mod = Module::GetSingleton();
		const auto version = mod->version().string("-"sv);
		const auto path = std::vformat(a_filename, std::make_format_args(version));
		if (!_mmap.open(path)) {
			const auto str = std::format("Failed to open Address Library file!\nPath: {}"sv, path);
			stl::report_and_fail(str);
		}

		_id2offset = std::span{
			reinterpret_cast<const mapping_t*>(_mmap.data() + sizeof(std::uint64_t)),
			*reinterpret_cast<const std::uint64_t*>(_mmap.data())
		};
	}
}
#endif

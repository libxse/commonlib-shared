#pragma once

#include "REL/Module.h"
#include "REL/Version.h"

#include "REX/REX/Singleton.h"

#ifndef XSE_MMIO_ADDRESSLIB
namespace REL
{
	class IDDB :
		public REX::Singleton<IDDB>
	{
	public:
		enum DatabaseVersion : std::uint32_t
		{
			kOriginal = 1,
			kLatest
		};

	private:
		struct mapping_t
		{
			std::uint64_t id;
			std::uint64_t offset;
		};

		class memory_map
		{
		public:
			memory_map() noexcept = default;
			memory_map(const memory_map&) = delete;

			memory_map(memory_map&& a_rhs) noexcept :
				_mapping(a_rhs._mapping),
				_view(a_rhs._view)
			{
				a_rhs._mapping = nullptr;
				a_rhs._view = nullptr;
			}

			~memory_map() { close(); }

			memory_map& operator=(const memory_map&) = delete;

			memory_map& operator=(memory_map&& a_rhs) noexcept
			{
				if (this != std::addressof(a_rhs)) {
					_mapping = a_rhs._mapping;
					a_rhs._mapping = nullptr;

					_view = a_rhs._view;
					a_rhs._view = nullptr;
				}
				return *this;
			}

			[[nodiscard]] void* data() noexcept { return _view; }

			bool open(stl::zwstring a_name, std::size_t a_size);
			bool create(stl::zwstring a_name, std::size_t a_size);
			void close();

		private:
			void* _mapping{ nullptr };
			void* _view{ nullptr };
		};

		class istream_t
		{
		public:
			using stream_type = std::ifstream;
			using pointer = stream_type*;
			using const_pointer = const stream_type*;
			using reference = stream_type&;
			using const_reference = const stream_type&;

			istream_t(const stl::zwstring a_filename, const std::ios_base::openmode a_mode) :
				_stream(a_filename.data(), a_mode)
			{
				if (!_stream.is_open()) {
					stl::report_and_fail("failed to open address library file"sv);
				}

				_stream.exceptions(std::ios::badbit | std::ios::failbit | std::ios::eofbit);
			}

			void ignore(const std::streamsize a_count)
			{
				_stream.ignore(a_count);
			}

			template <class T>
			void readin(T& a_val)
			{
				_stream.read(std::bit_cast<char*>(std::addressof(a_val)), sizeof(T));
			}

			template <class T>
				requires(std::is_arithmetic_v<T>)
			T readout()
			{
				T val{};
				readin(val);
				return val;
			}

		private:
			stream_type _stream;
		};

		class header_t
		{
		public:
			void read(istream_t& a_in, const DatabaseVersion a_version);

			[[nodiscard]] std::size_t      address_count() const noexcept { return static_cast<std::size_t>(_addressCount); }
			[[nodiscard]] std::uint64_t    pointer_size() const noexcept { return static_cast<std::uint64_t>(_pointerSize); }
			[[nodiscard]] std::string_view name() const noexcept { return _name; }
			[[nodiscard]] Version          version() const noexcept { return _version; }

		private:
			char         _name[260]{};
			Version      _version;
			std::int32_t _pointerSize{ 0 };
			std::int32_t _addressCount{ 0 };
		};

	public:
		IDDB() = default;

		void load(std::wstring_view a_filename, DatabaseVersion a_version = kLatest);

		[[nodiscard]] std::size_t id2offset(std::uint64_t a_id) const;

	protected:
		friend class Offset2ID;

		[[nodiscard]] std::span<mapping_t> get_id2offset() const noexcept { return _id2offset; }

	private:
		void unpack_file(istream_t& a_in, const header_t& a_header);

		memory_map           _mmap;
		std::span<mapping_t> _id2offset;
	};
}
#else
#include <mmio/mmio.hpp>

namespace REL
{
	class IDDB :
		public REX::Singleton<IDDB>
	{
	private:
		struct mapping_t
		{
			std::uint64_t id;
			std::uint64_t offset;
		};

	public:
		IDDB() = default;

		void load(std::string_view a_filename);

		[[nodiscard]] std::size_t id2offset(std::uint64_t a_id) const;

	protected:
		friend class Offset2ID;

		[[nodiscard]] std::span<const mapping_t> get_id2offset() const noexcept { return _id2offset; }

	private:
		mmio::mapped_file_source   _mmap;
		std::span<const mapping_t> _id2offset;
	};
}
#endif

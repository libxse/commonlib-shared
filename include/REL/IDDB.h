#pragma once

#include "REX/BASE.h"

#include "REL/Version.h"

#include "REX/REX/MemoryMap.h"
#include "REX/REX/Singleton.h"

namespace REL
{
	class IDDB :
		public REX::Singleton<IDDB>
	{
	public:
		enum class Loader : std::uint32_t
		{
			None,
			SKSE,
			F4SE,
			SFSE,
			//OBSE,
		};

		enum class Format : std::uint32_t
		{
			None = static_cast<std::uint32_t>(-1),
			V0 = 0,
			V1 = 1,
			V2 = 2,
		};

		IDDB();

		[[nodiscard]] std::size_t id2offset(std::uint64_t a_id) const;

	private:
		class istream_t;
		class header_t;

		struct mapping_t
		{
			std::uint64_t id;
			std::uint64_t offset;
		};

		void load();
		void load_v0();
		void unpack_file(istream_t& a_in, const header_t& a_header);

	protected:
		friend class Offset2ID;

		[[nodiscard]] std::span<mapping_t> get_id2offset() const noexcept { return m_id2offset; }

	private:
		Loader                m_loader{ Loader::None };
		Format                m_format{ Format::None };
		std::filesystem::path m_path;
		REX::MemoryMap        m_mmap;
		std::span<mapping_t>  m_id2offset;
	};
}

#pragma once

#include "REX/REX/Singleton.h"

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

		void load(std::string_view a_fileName);

		[[nodiscard]] std::size_t id2offset(std::uint64_t a_id) const;

	protected:
		friend class Offset2ID;

		[[nodiscard]] std::span<const mapping_t> get_id2offset() const noexcept { return _id2offset; }

	private:
		mmio::mapped_file_source   _mmap;
		std::span<const mapping_t> _id2offset;
	};
}

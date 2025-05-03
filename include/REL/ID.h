#pragma once

#include "REL/IDDB.h"
#include "REL/Module.h"

namespace REL
{
	class ID
	{
	public:
		constexpr ID() noexcept = default;

		explicit constexpr ID(std::uint64_t a_id) noexcept :
			_id(a_id)
		{}

		constexpr ID& operator=(std::uint64_t a_id) noexcept
		{
			_id = a_id;
			return *this;
		}

		[[nodiscard]] std::uintptr_t address() const
		{
			const auto mod = Module::GetSingleton();
			return mod->base() + offset();
		}

		[[nodiscard]] constexpr std::uint64_t id() const noexcept
		{
			return _id;
		}

		[[nodiscard]] std::size_t offset() const
		{
			const auto iddb = IDDB::GetSingleton();
			return iddb->id2offset(_id);
		}

	private:
		std::uint64_t _id{ static_cast<std::uint64_t>(-1) };
	};
}

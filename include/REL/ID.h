#pragma once

#include "REX/BASE.h"

#include "REL/IDDB.h"
#include "REL/Module.h"

namespace REL
{
	class ID
	{
	public:
		constexpr ID() noexcept = default;

		explicit constexpr ID(std::uint64_t a_id) noexcept :
			m_id(a_id)
		{}

		constexpr ID& operator=(std::uint64_t a_id) noexcept
		{
			m_id = a_id;
			return *this;
		}

		[[nodiscard]] std::uintptr_t address() const
		{
			const auto mod = Module::GetSingleton();
			return mod->base() + offset();
		}

		[[nodiscard]] constexpr std::uint64_t id() const noexcept
		{
			return m_id;
		}

		[[nodiscard]] std::size_t offset() const
		{
			const auto iddb = IDDB::GetSingleton();
			return iddb->offset(m_id);
		}

	private:
		std::uint64_t m_id{ 0 };
	};
}

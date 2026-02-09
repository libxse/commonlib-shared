#pragma once

#include "REX/FModule.h"

#include "REL/IDDB.h"

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
			const auto mod = REX::FModule::GetExecutingModule();
			return mod.GetBaseAddress() + offset();
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

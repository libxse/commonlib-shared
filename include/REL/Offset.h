#pragma once

#include "REX/REX/FModule.h"

namespace REL
{
	class Offset
	{
	public:
		constexpr Offset() noexcept = default;

		explicit constexpr Offset(std::size_t a_offset) noexcept :
			m_offset(a_offset)
		{}

		constexpr Offset& operator=(std::size_t a_offset) noexcept
		{
			m_offset = a_offset;
			return *this;
		}

		[[nodiscard]] std::uintptr_t address() const
		{
			const auto mod = REX::FModule::GetExecutingModule();
			return mod.GetBaseAddress() + offset();
		}

		[[nodiscard]] constexpr std::size_t offset() const noexcept
		{
			return m_offset;
		}

	private:
		std::size_t m_offset{ 0 };
	};
}

#pragma once

#include "REX/BASE.h"

#include "REL/Module.h"

namespace REL
{
	class Offset
	{
	public:
		constexpr Offset() noexcept = default;

		explicit constexpr Offset(std::size_t a_offset) noexcept :
			_offset(a_offset)
		{}

		constexpr Offset& operator=(std::size_t a_offset) noexcept
		{
			_offset = a_offset;
			return *this;
		}

		[[nodiscard]] std::uintptr_t address() const
		{
			const auto mod = Module::GetSingleton();
			return mod->base() + offset();
		}

		[[nodiscard]] constexpr std::size_t offset() const noexcept
		{
			return _offset;
		}

	private:
		std::size_t _offset{ 0 };
	};
}

#pragma once

#include <algorithm>
#include <array>
#include <atomic>
#include <bit>
#include <bitset>
#include <cassert>
#include <cmath>
#include <compare>
#include <concepts>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cwchar>
#include <cwctype>
#include <exception>
#include <execution>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <initializer_list>
#include <intrin.h>
#include <iomanip>
#include <ios>
#include <istream>
#include <iterator>
#include <limits>
#include <locale>
#include <map>
#include <memory>
#include <mutex>
#include <new>
#include <numeric>
#include <optional>
#include <queue>
#include <random>
#include <ranges>
#include <regex>
#include <set>
#include <source_location>
#include <span>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>
#include <thread>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <utility>
#include <variant>
#include <vector>

static_assert(std::is_integral_v<std::time_t> && sizeof(std::time_t) == sizeof(std::size_t), "wrap std::time_t instead");

namespace REX
{
	using namespace std::literals;
}

namespace REL
{
	using namespace std::literals;
}

namespace XSE
{
	using namespace std::literals;

	namespace stl
	{
		// owning pointer
		template <
			class T,
			class = std::enable_if_t<
				std::is_pointer_v<T>>>
		using owner = T;

		// non-owning pointer
		template <
			class T,
			class = std::enable_if_t<
				std::is_pointer_v<T>>>
		using observer = T;

		// non-null pointer
		template <
			class T,
			class = std::enable_if_t<
				std::is_pointer_v<T>>>
		using not_null = T;

		template <class T>
		struct remove_cvptr
		{
			using type = std::remove_cv_t<std::remove_pointer_t<T>>;
		};

		template <class T>
		using remove_cvptr_t = typename remove_cvptr<T>::type;

		template <class C, class K>
		concept transparent_comparator =
			requires(
				const K&                    a_transparent,
				const typename C::key_type& a_key,
				typename C::key_compare&    a_compare) {
				typename C::key_compare::is_transparent;
				// clang-format off
			{ a_compare(a_transparent, a_key) } -> std::convertible_to<bool>;
			{ a_compare(a_key, a_transparent) } -> std::convertible_to<bool>;
				// clang-format on
			};

		template <class EF>                                        //
			requires(std::invocable<std::remove_reference_t<EF>>)  //
		class scope_exit
		{
		public:
			// 1)
			template <class Fn>
			explicit scope_exit(Fn&& a_fn)  //
				noexcept(std::is_nothrow_constructible_v<EF, Fn> ||
						 std::is_nothrow_constructible_v<EF, Fn&>)  //
				requires(!std::is_same_v<std::remove_cvref_t<Fn>, scope_exit> &&
						 std::is_constructible_v<EF, Fn>)
			{
				static_assert(std::invocable<Fn>);

				if constexpr (!std::is_lvalue_reference_v<Fn> &&
							  std::is_nothrow_constructible_v<EF, Fn>) {
					_fn.emplace(std::forward<Fn>(a_fn));
				} else {
					_fn.emplace(a_fn);
				}
			}

			// 2)
			scope_exit(scope_exit&& a_rhs)  //
				noexcept(std::is_nothrow_move_constructible_v<EF> ||
						 std::is_nothrow_copy_constructible_v<EF>)  //
				requires(std::is_nothrow_move_constructible_v<EF> ||
						 std::is_copy_constructible_v<EF>)
			{
				static_assert(!(std::is_nothrow_move_constructible_v<EF> && !std::is_move_constructible_v<EF>));
				static_assert(!(!std::is_nothrow_move_constructible_v<EF> && !std::is_copy_constructible_v<EF>));

				if (a_rhs.active()) {
					if constexpr (std::is_nothrow_move_constructible_v<EF>) {
						_fn.emplace(std::forward<EF>(*a_rhs._fn));
					} else {
						_fn.emplace(a_rhs._fn);
					}
					a_rhs.release();
				}
			}

			// 3)
			scope_exit(const scope_exit&) = delete;

			~scope_exit() noexcept
			{
				if (_fn.has_value()) {
					(*_fn)();
				}
			}

			void release() noexcept { _fn.reset(); }

		private:
			[[nodiscard]] bool active() const noexcept { return _fn.has_value(); }

			std::optional<std::remove_reference_t<EF>> _fn;
		};

		template <class EF>
		scope_exit(EF) -> scope_exit<EF>;

		template <class F>
		class counted_function_iterator
		{
		public:
			using difference_type = std::ptrdiff_t;
			using value_type = std::remove_const_t<std::remove_reference_t<decltype(std::declval<F>()())>>;
			using pointer = value_type*;
			using reference = value_type&;
			using iterator_category = std::input_iterator_tag;

			counted_function_iterator() noexcept = default;

			counted_function_iterator(F a_fn, std::size_t a_count) noexcept :
				_fn(std::move(a_fn)),
				_left(a_count)
			{}

			[[nodiscard]] reference operator*() const  //
				noexcept(noexcept(std::declval<F>()()))
			{
				assert(_fn != std::nullopt);
				return (*_fn)();
			}

			[[nodiscard]] pointer operator->() const
			{
				return std::pointer_traits<pointer>::pointer_to(operator*());
			}

			[[nodiscard]] friend bool operator==(
				const counted_function_iterator& a_lhs,
				const counted_function_iterator& a_rhs) noexcept
			{
				return a_lhs._left == a_rhs._left;
			}

			counted_function_iterator& operator++() noexcept
			{
				assert(_left > 0);
				_left -= 1;
				return *this;
			}

			counted_function_iterator operator++(int) noexcept
			{
				counted_function_iterator tmp{ *this };
				operator++();
				return tmp;
			}

		private:
			std::optional<F> _fn;
			std::size_t      _left{ 0 };
		};
	}
}

namespace XSE
{
	namespace stl
	{
		template <class T>
		class atomic_ref :
			public std::atomic_ref<T>
		{
		private:
			using super = std::atomic_ref<T>;

		public:
			using value_type = typename super::value_type;

			explicit atomic_ref(volatile T& a_obj) noexcept(std::is_nothrow_constructible_v<super, value_type&>) :
				super(const_cast<value_type&>(a_obj))
			{}

			using super::super;
			using super::operator=;
		};

		template <class T>
		atomic_ref(volatile T&) -> atomic_ref<T>;

		template class atomic_ref<std::int8_t>;
		template class atomic_ref<std::uint8_t>;
		template class atomic_ref<std::int16_t>;
		template class atomic_ref<std::uint16_t>;
		template class atomic_ref<std::int32_t>;
		template class atomic_ref<std::uint32_t>;
		template class atomic_ref<std::int64_t>;
		template class atomic_ref<std::uint64_t>;

		static_assert(atomic_ref<std::int8_t>::is_always_lock_free);
		static_assert(atomic_ref<std::uint8_t>::is_always_lock_free);
		static_assert(atomic_ref<std::int16_t>::is_always_lock_free);
		static_assert(atomic_ref<std::uint16_t>::is_always_lock_free);
		static_assert(atomic_ref<std::int32_t>::is_always_lock_free);
		static_assert(atomic_ref<std::uint32_t>::is_always_lock_free);
		static_assert(atomic_ref<std::int64_t>::is_always_lock_free);
		static_assert(atomic_ref<std::uint64_t>::is_always_lock_free);

		template <class T>
		struct ssizeof
		{
			[[nodiscard]] constexpr operator std::ptrdiff_t() const noexcept { return value; }

			[[nodiscard]] constexpr std::ptrdiff_t operator()() const noexcept { return value; }

			static constexpr auto value = static_cast<std::ptrdiff_t>(sizeof(T));
		};

		template <class T>
		inline constexpr auto ssizeof_v = ssizeof<T>::value;

		template <class T>
		void memzero(volatile T* a_ptr, std::size_t a_size = sizeof(T))
		{
			const auto     begin = reinterpret_cast<volatile char*>(a_ptr);
			constexpr char val{ 0 };
			std::fill_n(begin, a_size, val);
		}

		template <class... Args>
		[[nodiscard]] inline auto pun_bits(Args... a_args)  //
			requires(std::same_as<std::remove_cv_t<Args>, bool> && ...)
		{
			constexpr auto ARGC = sizeof...(Args);

			std::bitset<ARGC> bits;
			std::size_t       i = 0;
			((bits[i++] = a_args), ...);

			if constexpr (ARGC <= std::numeric_limits<unsigned long>::digits) {
				return bits.to_ulong();
			} else if constexpr (ARGC <= std::numeric_limits<unsigned long long>::digits) {
				return bits.to_ullong();
			} else {
				static_assert(false && sizeof...(Args));
			}
		}
	}
}

#pragma once

#include "REX/BASE.h"

#include "REL/IDDB.h"

namespace REL
{
	class Offset2ID :
		public REX::Singleton<Offset2ID>
	{
	public:
		using value_type = IDDB::MAPPING;
		using container_type = std::vector<value_type>;
		using size_type = typename container_type::size_type;
		using const_iterator = typename container_type::const_iterator;
		using const_reverse_iterator = typename container_type::const_reverse_iterator;

		void load(std::span<IDDB::MAPPING> a_span)
		{
			const auto iddb = IDDB::GetSingleton();
			const auto id2offset = iddb->get_id2offset<IDDB::MAPPING>();
			_offset2id.reserve(id2offset.size());
			_offset2id.insert(_offset2id.begin(), id2offset.begin(), id2offset.end());
			std::sort(std::execution::sequenced_policy{}, _offset2id.begin(), _offset2id.end(), [](auto&& a_lhs, auto&& a_rhs) {
				return a_lhs.offset < a_rhs.offset;
			});
		}

		void load(std::span<std::uint32_t> a_span)
		{
			const auto iddb = IDDB::GetSingleton();
			const auto id2offset = iddb->get_id2offset<std::uint32_t>();
			_offset2id.reserve(id2offset.size());

			std::uint64_t id{ 0 };
			for (auto offset : id2offset) {
				value_type map{ id++, offset };
				_offset2id.emplace_back(map);
			}
			std::sort(std::execution::sequenced_policy{}, _offset2id.begin(), _offset2id.end(), [](auto&& a_lhs, auto&& a_rhs) {
				return a_lhs.offset < a_rhs.offset;
			});
		}

		[[nodiscard]] std::uint64_t operator()(std::size_t a_offset) const
		{
			if (_offset2id.empty()) {
				REX::FAIL("No Address Library has been loaded!");
			}

			const value_type elem{ 0, a_offset };
			const auto       it = std::lower_bound(
                _offset2id.begin(),
                _offset2id.end(),
                elem,
                [](auto&& a_lhs, auto&& a_rhs) {
                    return a_lhs.offset < a_rhs.offset;
                });
			if (it == _offset2id.end()) {
				const auto mod = Module::GetSingleton();
				const auto version = mod->version();
				REX::FAIL(
					"Failed to find Address Library ID for offset!\n"
					"Invalid offset: 0x{:08X}\n"
					"Game Version: {}",
					a_offset, version.string());
			}

			return it->id;
		}

		[[nodiscard]] const_iterator begin() const noexcept { return _offset2id.begin(); }
		[[nodiscard]] const_iterator cbegin() const noexcept { return _offset2id.cbegin(); }

		[[nodiscard]] const_iterator end() const noexcept { return _offset2id.end(); }
		[[nodiscard]] const_iterator cend() const noexcept { return _offset2id.cend(); }

		[[nodiscard]] const_reverse_iterator rbegin() const noexcept { return _offset2id.rbegin(); }
		[[nodiscard]] const_reverse_iterator crbegin() const noexcept { return _offset2id.crbegin(); }

		[[nodiscard]] const_reverse_iterator rend() const noexcept { return _offset2id.rend(); }
		[[nodiscard]] const_reverse_iterator crend() const noexcept { return _offset2id.crend(); }

		[[nodiscard]] size_type size() const noexcept { return _offset2id.size(); }

	private:
		container_type _offset2id;
	};
}

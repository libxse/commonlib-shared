#ifdef COMMONLIB_OPTION_TOML

#	include "REX/TTomlSetting.h"

#	include <toml.hpp>

toml::value* toml_recurse_table(toml::value* a_result, toml::value& a_value, const std::string& a_section, bool a_create)
{
	if (a_result && a_result->is_table()) {
		for (auto& value : a_result->as_table()) {
			if (value.first == a_section) {
				return std::addressof(value.second);
			}
		}
		if (a_create) {
			(*a_result)[a_section] = toml::table{};
			return std::addressof((*a_result)[a_section]);
		}
	} else if (a_value.is_table()) {
		for (auto& value : a_value.as_table()) {
			if (value.first == a_section) {
				return std::addressof(value.second);
			}
		}
		if (a_create) {
			a_value[a_section] = toml::table{};
			return std::addressof(a_value[a_section]);
		}
	}
	return a_result;
}

namespace REX::Impl
{
	template <class T>
	void TomlSettingLoad<T>(
		void*                    a_data,
		std::vector<std::string> a_section,
		std::string_view         a_key,
		T&                       a_value,
		T&                       a_valueDefault)
	{
		const auto& data = static_cast<toml::value*>(a_data);
		if (a_section.empty()) {
			auto& path = (*data);
			a_value = toml::find_or<T>(path, a_key.data(), a_valueDefault);
			return;
		} else if (a_section.size() == 1) {
			auto& path = (*data)[a_section.front()];
			a_value = toml::find_or<T>(path, a_key.data(), a_valueDefault);
			return;
		} else {
			toml::value* path{ nullptr };
			for (auto& section : a_section) {
				path = toml_recurse_table(path, *data, section, false);
			}
			if (path) {
				a_value = toml::find_or<T>(*path, a_key.data(), a_valueDefault);
				return;
			}
		}
		a_value = a_valueDefault;
	}

	template void TomlSettingLoad<bool>(void*, std::vector<std::string>, std::string_view, bool&, bool&);
	template void TomlSettingLoad<float>(void*, std::vector<std::string>, std::string_view, float&, float&);
	template void TomlSettingLoad<double>(void*, std::vector<std::string>, std::string_view, double&, double&);
	template void TomlSettingLoad<std::uint8_t>(void*, std::vector<std::string>, std::string_view, std::uint8_t&, std::uint8_t&);
	template void TomlSettingLoad<std::uint16_t>(void*, std::vector<std::string>, std::string_view, std::uint16_t&, std::uint16_t&);
	template void TomlSettingLoad<std::uint32_t>(void*, std::vector<std::string>, std::string_view, std::uint32_t&, std::uint32_t&);
	template void TomlSettingLoad<std::int8_t>(void*, std::vector<std::string>, std::string_view, std::int8_t&, std::int8_t&);
	template void TomlSettingLoad<std::int16_t>(void*, std::vector<std::string>, std::string_view, std::int16_t&, std::int16_t&);
	template void TomlSettingLoad<std::int32_t>(void*, std::vector<std::string>, std::string_view, std::int32_t&, std::int32_t&);
	template void TomlSettingLoad<std::string>(void*, std::vector<std::string>, std::string_view, std::string&, std::string&);

	template <class T>
	void TomlSettingSave<T>(
		void*                    a_data,
		std::vector<std::string> a_section,
		std::string_view         a_key,
		T&                       a_value)
	{
		auto& data = *static_cast<toml::value*>(a_data);
		if (a_section.empty()) {
			data[a_key.data()] = a_value;
		} else if (a_section.size() == 1) {
			data[a_section.front()][a_key.data()] = a_value;
		} else {
			toml::value* path{ nullptr };
			for (auto& section : a_section) {
				path = toml_recurse_table(path, data, section, true);
			}
			if (path) {
				(*path)[a_key.data()] = a_value;
			}
		}
	}

	template void TomlSettingSave<bool>(void*, std::vector<std::string>, std::string_view, bool&);
	template void TomlSettingSave<float>(void*, std::vector<std::string>, std::string_view, float&);
	template void TomlSettingSave<double>(void*, std::vector<std::string>, std::string_view, double&);
	template void TomlSettingSave<std::uint8_t>(void*, std::vector<std::string>, std::string_view, std::uint8_t&);
	template void TomlSettingSave<std::uint16_t>(void*, std::vector<std::string>, std::string_view, std::uint16_t&);
	template void TomlSettingSave<std::uint32_t>(void*, std::vector<std::string>, std::string_view, std::uint32_t&);
	template void TomlSettingSave<std::int8_t>(void*, std::vector<std::string>, std::string_view, std::int8_t&);
	template void TomlSettingSave<std::int16_t>(void*, std::vector<std::string>, std::string_view, std::int16_t&);
	template void TomlSettingSave<std::int32_t>(void*, std::vector<std::string>, std::string_view, std::int32_t&);
	template void TomlSettingSave<std::string>(void*, std::vector<std::string>, std::string_view, std::string&);
}
#endif

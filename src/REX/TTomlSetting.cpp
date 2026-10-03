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

void toml_load_description(toml::value a_path, std::vector<std::string>& a_description)
{
	if (!a_path.comments().empty()) {
		a_description.clear();
		std::ranges::transform(a_path.comments(), std::back_inserter(a_description),
			[](std::string a_string) {
				if (a_string.front() == '#') {
					a_string.erase(a_string.begin());
				}
				return a_string;
			});
	}
}

void toml_save_description(toml::value& a_path, std::vector<std::string> a_description)
{
	if (!a_description.empty()) {
		a_path.comments().clear();
		std::ranges::copy(a_description, std::back_inserter(a_path.comments()));
	}
}

namespace REX::Impl
{
	template <class T>
	void TomlSettingLoadEx(void* a_data, std::vector<std::string> a_section, std::string_view a_key, T& a_value, T& a_valueDefault, std::vector<std::string>& a_description)
	{
		const auto& data = static_cast<toml::value*>(a_data);
		if (a_section.empty()) {
			auto& path = (*data);
			a_value = toml::find_or<T>(path, a_key.data(), a_valueDefault);
			toml_load_description(path[a_key.data()], a_description);
			return;
		} else if (a_section.size() == 1) {
			auto& path = (*data)[a_section.front()];
			a_value = toml::find_or<T>(path, a_key.data(), a_valueDefault);
			toml_load_description(path[a_key.data()], a_description);
			return;
		} else {
			toml::value* path{ nullptr };
			for (auto& section : a_section) {
				path = toml_recurse_table(path, *data, section, false);
			}
			if (path) {
				a_value = toml::find_or<T>(*path, a_key.data(), a_valueDefault);
				toml_load_description((*path)[a_key.data()], a_description);
				return;
			}
		}
		a_value = a_valueDefault;
	}

	template <>
	void TomlSettingLoad<bool>(void* a_data, std::vector<std::string> a_section, std::string_view a_key, bool& a_value, bool& a_valueDefault, std::vector<std::string>& a_description)
	{
		TomlSettingLoadEx<bool>(a_data, a_section, a_key, a_value, a_valueDefault, a_description);
	}

	template <>
	void TomlSettingLoad<float>(void* a_data, std::vector<std::string> a_section, std::string_view a_key, float& a_value, float& a_valueDefault, std::vector<std::string>& a_description)
	{
		TomlSettingLoadEx<float>(a_data, a_section, a_key, a_value, a_valueDefault, a_description);
	}

	template <>
	void TomlSettingLoad<double>(void* a_data, std::vector<std::string> a_section, std::string_view a_key, double& a_value, double& a_valueDefault, std::vector<std::string>& a_description)
	{
		TomlSettingLoadEx<double>(a_data, a_section, a_key, a_value, a_valueDefault, a_description);
	}

	template <>
	void TomlSettingLoad<std::uint8_t>(void* a_data, std::vector<std::string> a_section, std::string_view a_key, std::uint8_t& a_value, std::uint8_t& a_valueDefault, std::vector<std::string>& a_description)
	{
		TomlSettingLoadEx<std::uint8_t>(a_data, a_section, a_key, a_value, a_valueDefault, a_description);
	}

	template <>
	void TomlSettingLoad<std::uint16_t>(void* a_data, std::vector<std::string> a_section, std::string_view a_key, std::uint16_t& a_value, std::uint16_t& a_valueDefault, std::vector<std::string>& a_description)
	{
		TomlSettingLoadEx<std::uint16_t>(a_data, a_section, a_key, a_value, a_valueDefault, a_description);
	}

	template <>
	void TomlSettingLoad<std::uint32_t>(void* a_data, std::vector<std::string> a_section, std::string_view a_key, std::uint32_t& a_value, std::uint32_t& a_valueDefault, std::vector<std::string>& a_description)
	{
		TomlSettingLoadEx<std::uint32_t>(a_data, a_section, a_key, a_value, a_valueDefault, a_description);
	}

	template <>
	void TomlSettingLoad<std::int8_t>(void* a_data, std::vector<std::string> a_section, std::string_view a_key, std::int8_t& a_value, std::int8_t& a_valueDefault, std::vector<std::string>& a_description)
	{
		TomlSettingLoadEx<std::int8_t>(a_data, a_section, a_key, a_value, a_valueDefault, a_description);
	}

	template <>
	void TomlSettingLoad<std::int16_t>(void* a_data, std::vector<std::string> a_section, std::string_view a_key, std::int16_t& a_value, std::int16_t& a_valueDefault, std::vector<std::string>& a_description)
	{
		TomlSettingLoadEx<std::int16_t>(a_data, a_section, a_key, a_value, a_valueDefault, a_description);
	}

	template <>
	void TomlSettingLoad<std::int32_t>(void* a_data, std::vector<std::string> a_section, std::string_view a_key, std::int32_t& a_value, std::int32_t& a_valueDefault, std::vector<std::string>& a_description)
	{
		TomlSettingLoadEx<std::int32_t>(a_data, a_section, a_key, a_value, a_valueDefault, a_description);
	}

	template <>
	void TomlSettingLoad<std::string>(void* a_data, std::vector<std::string> a_section, std::string_view a_key, std::string& a_value, std::string& a_valueDefault, std::vector<std::string>& a_description)
	{
		TomlSettingLoadEx<std::string>(a_data, a_section, a_key, a_value, a_valueDefault, a_description);
	}

	template <class T>
	void TomlSettingSaveEx(void* a_data, std::vector<std::string> a_section, std::string_view a_key, T& a_value, std::vector<std::string> a_description)
	{
		auto& data = *static_cast<toml::value*>(a_data);
		if (a_section.empty()) {
			data[a_key.data()] = a_value;
			toml_save_description(data[a_key.data()], a_description);
		} else if (a_section.size() == 1) {
			data[a_section.front()][a_key.data()] = a_value;
			toml_save_description(data[a_section.front()][a_key.data()], a_description);
		} else {
			toml::value* path{ nullptr };
			for (auto& section : a_section) {
				path = toml_recurse_table(path, data, section, true);
			}
			if (path) {
				(*path)[a_key.data()] = a_value;
				toml_save_description((*path)[a_key.data()], a_description);
			}
		}
	}

	template <>
	void TomlSettingSave<bool>(void* a_data, std::vector<std::string> a_section, std::string_view a_key, bool& a_value, std::vector<std::string> a_description)
	{
		TomlSettingSaveEx<bool>(a_data, a_section, a_key, a_value, a_description);
	}

	template <>
	void TomlSettingSave<float>(void* a_data, std::vector<std::string> a_section, std::string_view a_key, float& a_value, std::vector<std::string> a_description)
	{
		TomlSettingSaveEx<float>(a_data, a_section, a_key, a_value, a_description);
	}

	template <>
	void TomlSettingSave<double>(void* a_data, std::vector<std::string> a_section, std::string_view a_key, double& a_value, std::vector<std::string> a_description)
	{
		TomlSettingSaveEx<double>(a_data, a_section, a_key, a_value, a_description);
	}

	template <>
	void TomlSettingSave<std::uint8_t>(void* a_data, std::vector<std::string> a_section, std::string_view a_key, std::uint8_t& a_value, std::vector<std::string> a_description)
	{
		TomlSettingSaveEx<std::uint8_t>(a_data, a_section, a_key, a_value, a_description);
	}

	template <>
	void TomlSettingSave<std::uint16_t>(void* a_data, std::vector<std::string> a_section, std::string_view a_key, std::uint16_t& a_value, std::vector<std::string> a_description)
	{
		TomlSettingSaveEx<std::uint16_t>(a_data, a_section, a_key, a_value, a_description);
	}

	template <>
	void TomlSettingSave<std::uint32_t>(void* a_data, std::vector<std::string> a_section, std::string_view a_key, std::uint32_t& a_value, std::vector<std::string> a_description)
	{
		TomlSettingSaveEx<std::uint32_t>(a_data, a_section, a_key, a_value, a_description);
	}

	template <>
	void TomlSettingSave<std::int8_t>(void* a_data, std::vector<std::string> a_section, std::string_view a_key, std::int8_t& a_value, std::vector<std::string> a_description)
	{
		TomlSettingSaveEx<std::int8_t>(a_data, a_section, a_key, a_value, a_description);
	}

	template <>
	void TomlSettingSave<std::int16_t>(void* a_data, std::vector<std::string> a_section, std::string_view a_key, std::int16_t& a_value, std::vector<std::string> a_description)
	{
		TomlSettingSaveEx<std::int16_t>(a_data, a_section, a_key, a_value, a_description);
	}

	template <>
	void TomlSettingSave<std::int32_t>(void* a_data, std::vector<std::string> a_section, std::string_view a_key, std::int32_t& a_value, std::vector<std::string> a_description)
	{
		TomlSettingSaveEx<std::int32_t>(a_data, a_section, a_key, a_value, a_description);
	}

	template <>
	void TomlSettingSave<std::string>(void* a_data, std::vector<std::string> a_section, std::string_view a_key, std::string& a_value, std::vector<std::string> a_description)
	{
		TomlSettingSaveEx<std::string>(a_data, a_section, a_key, a_value, a_description);
	}
}
#endif

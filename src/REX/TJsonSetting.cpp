#ifdef COMMONLIB_OPTION_JSON

#	include "REX/TJsonSetting.h"

#	include <glaze/glaze.hpp>

namespace REX::Impl
{
	template <class T>
	void JsonSettingLoad<T>(
		void*            a_data,
		std::string_view a_path,
		T&               a_value,
		T&               a_valueDefault)
	{
		const auto& json = *static_cast<glz::generic*>(a_data);
		if (a_path[0] != '/') {
			const auto path = std::format("/{}"sv, a_path);
			a_value = glz::get<T>(json, path).value_or(a_valueDefault);
		} else {
			a_value = glz::get<T>(json, a_path).value_or(a_valueDefault);
		}
	}

	template void JsonSettingLoad<bool>(void*, std::string_view, bool&, bool&);
	//template void JsonSettingLoad<float>(void*, std::string_view, float&, float&);
	template void JsonSettingLoad<double>(void*, std::string_view, double&, double&);
	template void JsonSettingLoad<std::uint8_t>(void*, std::string_view, std::uint8_t&, std::uint8_t&);
	template void JsonSettingLoad<std::uint16_t>(void*, std::string_view, std::uint16_t&, std::uint16_t&);
	template void JsonSettingLoad<std::uint32_t>(void*, std::string_view, std::uint32_t&, std::uint32_t&);
	template void JsonSettingLoad<std::int8_t>(void*, std::string_view, std::int8_t&, std::int8_t&);
	template void JsonSettingLoad<std::int16_t>(void*, std::string_view, std::int16_t&, std::int16_t&);
	template void JsonSettingLoad<std::int32_t>(void*, std::string_view, std::int32_t&, std::int32_t&);
	template void JsonSettingLoad<std::string>(void*, std::string_view, std::string&, std::string&);
	template void JsonSettingLoad<std::vector<bool>>(void*, std::string_view, std::vector<bool>&, std::vector<bool>&);
	//template void JsonSettingLoad<std::vector<float>>(void*, std::string_view, std::vector<float>&, std::vector<float>&);
	template void JsonSettingLoad<std::vector<double>>(void*, std::string_view, std::vector<double>&, std::vector<double>&);
	template void JsonSettingLoad<std::vector<std::uint8_t>>(void*, std::string_view, std::vector<std::uint8_t>&, std::vector<std::uint8_t>&);
	template void JsonSettingLoad<std::vector<std::uint16_t>>(void*, std::string_view, std::vector<std::uint16_t>&, std::vector<std::uint16_t>&);
	template void JsonSettingLoad<std::vector<std::uint32_t>>(void*, std::string_view, std::vector<std::uint32_t>&, std::vector<std::uint32_t>&);
	template void JsonSettingLoad<std::vector<std::int8_t>>(void*, std::string_view, std::vector<std::int8_t>&, std::vector<std::int8_t>&);
	template void JsonSettingLoad<std::vector<std::int16_t>>(void*, std::string_view, std::vector<std::int16_t>&, std::vector<std::int16_t>&);
	template void JsonSettingLoad<std::vector<std::int32_t>>(void*, std::string_view, std::vector<std::int32_t>&, std::vector<std::int32_t>&);
	template void JsonSettingLoad<std::vector<std::string>>(void*, std::string_view, std::vector<std::string>&, std::vector<std::string>&);

	template <class T>
	void JsonSettingSave<T>(
		void*            a_data,
		std::string_view a_path,
		T&               a_value)
	{
		auto& json = *static_cast<glz::generic*>(a_data);
		if (a_path[0] != '/') {
			const auto path = std::format("/{}"sv, a_path);
			glz::set(json, path, a_value);
		} else {
			glz::set(json, a_path, a_value);
		}
	}

	template void JsonSettingSave<bool>(void*, std::string_view, bool&);
	//template void JsonSettingSave<float>(void*, std::string_view, float&);
	template void JsonSettingSave<double>(void*, std::string_view, double&);
	template void JsonSettingSave<std::uint8_t>(void*, std::string_view, std::uint8_t&);
	template void JsonSettingSave<std::uint16_t>(void*, std::string_view, std::uint16_t&);
	template void JsonSettingSave<std::uint32_t>(void*, std::string_view, std::uint32_t&);
	template void JsonSettingSave<std::int8_t>(void*, std::string_view, std::int8_t&);
	template void JsonSettingSave<std::int16_t>(void*, std::string_view, std::int16_t&);
	template void JsonSettingSave<std::int32_t>(void*, std::string_view, std::int32_t&);
	template void JsonSettingSave<std::string>(void*, std::string_view, std::string&);
	template void JsonSettingSave<std::vector<bool>>(void*, std::string_view, std::vector<bool>&);
	//template void JsonSettingSave<std::vector<float>>(void*, std::string_view, std::vector<float>&);
	template void JsonSettingSave<std::vector<double>>(void*, std::string_view, std::vector<double>&);
	template void JsonSettingSave<std::vector<std::uint8_t>>(void*, std::string_view, std::vector<std::uint8_t>&);
	template void JsonSettingSave<std::vector<std::uint16_t>>(void*, std::string_view, std::vector<std::uint16_t>&);
	template void JsonSettingSave<std::vector<std::uint32_t>>(void*, std::string_view, std::vector<std::uint32_t>&);
	template void JsonSettingSave<std::vector<std::int8_t>>(void*, std::string_view, std::vector<std::int8_t>&);
	template void JsonSettingSave<std::vector<std::int16_t>>(void*, std::string_view, std::vector<std::int16_t>&);
	template void JsonSettingSave<std::vector<std::int32_t>>(void*, std::string_view, std::vector<std::int32_t>&);
	template void JsonSettingSave<std::vector<std::string>>(void*, std::string_view, std::vector<std::string>&);
}

#endif

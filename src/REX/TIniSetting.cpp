#ifdef COMMONLIB_OPTION_INI

#include "REX/TIniSetting.h"

#include <SimpleIni.h>
#undef ERROR

namespace REX::Impl
{
	template <class T>
	constexpr bool is_long_integral_v = std::_Is_any_of_v<std::remove_cv_t<T>, std::uint8_t, std::uint16_t, std::uint32_t, std::int8_t, std::int16_t, std::int32_t>;

	template <class T>
	void IniSettingLoad<T>(
		void*            a_data,
		std::string_view a_section,
		std::string_view a_key,
		T&               a_value,
		T&               a_valueDefault)
	{
		const auto file = static_cast<CSimpleIniA*>(a_data);
		if constexpr (std::is_same_v<T, bool>) {
			a_value = file->GetBoolValue(a_section.data(), a_key.data(), a_valueDefault);
		} else if constexpr (std::is_floating_point_v<T>) {
			a_value = static_cast<T>(file->GetDoubleValue(a_section.data(), a_key.data(), a_valueDefault));
		} else if constexpr (is_long_integral_v<T>) {
			a_value = static_cast<T>(file->GetLongValue(a_section.data(), a_key.data(), a_valueDefault));
		} else if constexpr (std::is_same_v<T, std::string>) {
			a_value = file->GetValue(a_section.data(), a_key.data(), a_valueDefault.c_str());
		}
	}

	template void IniSettingLoad<bool>(void*, std::string_view, std::string_view, bool&, bool&);
	template void IniSettingLoad<float>(void*, std::string_view, std::string_view, float&, float&);
	template void IniSettingLoad<double>(void*, std::string_view, std::string_view, double&, double&);
	template void IniSettingLoad<std::uint8_t>(void*, std::string_view, std::string_view, std::uint8_t&, std::uint8_t&);
	template void IniSettingLoad<std::uint16_t>(void*, std::string_view, std::string_view, std::uint16_t&, std::uint16_t&);
	template void IniSettingLoad<std::uint32_t>(void*, std::string_view, std::string_view, std::uint32_t&, std::uint32_t&);
	template void IniSettingLoad<std::int8_t>(void*, std::string_view, std::string_view, std::int8_t&, std::int8_t&);
	template void IniSettingLoad<std::int16_t>(void*, std::string_view, std::string_view, std::int16_t&, std::int16_t&);
	template void IniSettingLoad<std::int32_t>(void*, std::string_view, std::string_view, std::int32_t&, std::int32_t&);
	template void IniSettingLoad<std::string>(void*, std::string_view, std::string_view, std::string&, std::string&);

	template <class T>
	void IniSettingSave<T>(
		void*            a_data,
		std::string_view a_section,
		std::string_view a_key,
		T&               a_value)
	{
		auto& file = *static_cast<CSimpleIniA*>(a_data);
		if constexpr (std::is_same_v<T, bool>) {
			file.SetBoolValue(a_section.data(), a_key.data(), a_value);
		} else if constexpr (std::is_floating_point_v<T>) {
			file.SetDoubleValue(a_section.data(), a_key.data(), a_value);
		} else if constexpr (is_long_integral_v<T>) {
			file.SetLongValue(a_section.data(), a_key.data(), a_value);
		} else if constexpr (std::is_same_v<T, std::string>) {
			file.SetValue(a_section.data(), a_key.data(), a_value.c_str());
		}
	}

	template void IniSettingSave<bool>(void*, std::string_view, std::string_view, bool&);
	template void IniSettingSave<float>(void*, std::string_view, std::string_view, float&);
	template void IniSettingSave<double>(void*, std::string_view, std::string_view, double&);
	template void IniSettingSave<std::uint8_t>(void*, std::string_view, std::string_view, std::uint8_t&);
	template void IniSettingSave<std::uint16_t>(void*, std::string_view, std::string_view, std::uint16_t&);
	template void IniSettingSave<std::uint32_t>(void*, std::string_view, std::string_view, std::uint32_t&);
	template void IniSettingSave<std::int8_t>(void*, std::string_view, std::string_view, std::int8_t&);
	template void IniSettingSave<std::int16_t>(void*, std::string_view, std::string_view, std::int16_t&);
	template void IniSettingSave<std::int32_t>(void*, std::string_view, std::string_view, std::int32_t&);
	template void IniSettingSave<std::string>(void*, std::string_view, std::string_view, std::string&);
}
#endif

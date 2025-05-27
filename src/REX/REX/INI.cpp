#include "REX/REX/INI.h"

#ifdef COMMONLIB_OPTION_INI
#	include <SimpleIni.h>
#	undef ERROR

namespace REX::INI
{
	namespace Impl
	{
		template <class T>
		constexpr bool is_long_integral_v = std::_Is_any_of_v<std::remove_cv_t<T>, std::uint8_t, std::uint16_t, std::uint32_t, std::int8_t, std::int16_t, std::int32_t>;

		template <class T>
		void SettingLoad<T>(
			void* a_data,
			sec_t a_section,
			key_t a_key,
			T&    a_value,
			T&    a_valueDefault)
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

		template void SettingLoad<bool>(void*, sec_t, key_t, bool&, bool&);
		template void SettingLoad<float>(void*, sec_t, key_t, float&, float&);
		template void SettingLoad<double>(void*, sec_t, key_t, double&, double&);
		template void SettingLoad<std::uint8_t>(void*, sec_t, key_t, std::uint8_t&, std::uint8_t&);
		template void SettingLoad<std::uint16_t>(void*, sec_t, key_t, std::uint16_t&, std::uint16_t&);
		template void SettingLoad<std::uint32_t>(void*, sec_t, key_t, std::uint32_t&, std::uint32_t&);
		template void SettingLoad<std::int8_t>(void*, sec_t, key_t, std::int8_t&, std::int8_t&);
		template void SettingLoad<std::int16_t>(void*, sec_t, key_t, std::int16_t&, std::int16_t&);
		template void SettingLoad<std::int32_t>(void*, sec_t, key_t, std::int32_t&, std::int32_t&);
		template void SettingLoad<std::string>(void*, sec_t, key_t, std::string&, std::string&);

		template <class T>
		void SettingSave<T>(
			void* a_data,
			sec_t a_section,
			key_t a_key,
			T&    a_value)
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

		template void SettingSave<bool>(void*, sec_t, key_t, bool&);
		template void SettingSave<float>(void*, sec_t, key_t, float&);
		template void SettingSave<double>(void*, sec_t, key_t, double&);
		template void SettingSave<std::uint8_t>(void*, sec_t, key_t, std::uint8_t&);
		template void SettingSave<std::uint16_t>(void*, sec_t, key_t, std::uint16_t&);
		template void SettingSave<std::uint32_t>(void*, sec_t, key_t, std::uint32_t&);
		template void SettingSave<std::int8_t>(void*, sec_t, key_t, std::int8_t&);
		template void SettingSave<std::int16_t>(void*, sec_t, key_t, std::int16_t&);
		template void SettingSave<std::int32_t>(void*, sec_t, key_t, std::int32_t&);
		template void SettingSave<std::string>(void*, sec_t, key_t, std::string&);
	}

	void SettingStore::Load()
	{
		CSimpleIniA file;
		file.SetUnicode(true);
		file.SetQuotes(true);

		if (std::filesystem::exists(m_fileBase)) {
			file.LoadFile(m_fileBase.data());
			for (auto& setting : m_settings) {
				setting->Load(&file, true);
			}
		}

		if (std::filesystem::exists(m_fileUser)) {
			file.LoadFile(m_fileUser.data());
			for (auto& setting : m_settings) {
				setting->Load(&file, false);
			}
		}
	}

	void SettingStore::Save()
	{
		CSimpleIniA file;
		file.SetUnicode(true);
		file.SetQuotes(true);

		file.LoadFile(m_fileBase.data());
		for (auto& setting : m_settings) {
			setting->Save(&file);
		}

		file.SaveFile(m_fileBase.data());
	}
}
#endif

#ifdef COMMONLIB_OPTION_INI

#	include "REX/TIniSettingA.h"

#	include <SimpleIni.h>
#	undef ERROR

namespace REX::Impl
{
	template <>
	void IniSettingLoad<std::vector<std::string>>(void* a_data, std::string_view a_section, std::string_view a_key, std::vector<std::string>& a_value, std::vector<std::string>& a_valueDefault, std::string_view a_delimiter)
	{
		const auto data = static_cast<CSimpleIniA*>(a_data);
		a_value = REX::STR::SPLIT(data->GetValue(a_section.data(), a_key.data(), REX::STR::JOIN(a_valueDefault, a_delimiter).c_str()), a_delimiter);
	}

	template <>
	void IniSettingSave<std::vector<std::string>>(void* a_data, std::string_view a_section, std::string_view a_key, std::vector<std::string>& a_value, std::string_view a_delimiter)
	{
		auto& data = *static_cast<CSimpleIniA*>(a_data);
		data.SetValue(a_section.data(), a_key.data(), REX::STR::JOIN(a_value, a_delimiter).c_str());
	}
}
#endif

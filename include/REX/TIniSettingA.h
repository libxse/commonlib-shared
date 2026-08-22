#pragma once

#ifdef COMMONLIB_OPTION_INI

#	include "REX/FIniSettingStore.h"
#   include "REX/STRING.h"
#	include "REX/TSetting.h"

namespace REX::Impl
{
	template <class T>
	void IniSettingLoad(void* a_data, std::string_view a_section, std::string_view a_key, T& a_value, T& a_valueDefault, std::string_view a_delimiter);

	template <class T>
	void IniSettingSave(void* a_data, std::string_view a_section, std::string_view a_key, T& a_value, std::string_view a_delimiter);
}

namespace REX
{
	template <class T, class S = FIniSettingStore>
	class TIniSettingA :
		public TSetting<std::vector<T>>
	{
	public:
		TIniSettingA(std::string_view a_section, std::string_view a_key, std::string a_default, std::string_view a_delimiter = R"(|)"sv) :
			TSetting<std::vector<T>>(REX::STR::SPLIT(a_default, a_delimiter)),
			m_section(a_section),
			m_key(a_key),
			m_delimiter(a_delimiter)
		{
			S::GetSingleton()->Add(this);
		}

		TIniSettingA(std::string_view a_key, std::string a_default, std::string_view a_delimiter = R"(|)"sv) :
			TSetting<std::vector<T>>(REX::STR::SPLIT(a_default, a_delimiter)),
			m_section(),
			m_key(a_key),
			m_delimiter(a_delimiter)
		{
			S::GetSingleton()->Add(this);
		}

	public:
		virtual void Load(void* a_data, bool a_isBase) override
		{
			if (a_isBase) {
				Impl::IniSettingLoad<std::vector<T>>(a_data, m_section, m_key, this->m_valueDefault, this->m_valueDefault, m_delimiter);
				this->SetValue(this->m_valueDefault);
			} else {
				Impl::IniSettingLoad<std::vector<T>>(a_data, m_section, m_key, this->m_value, this->m_valueDefault, m_delimiter);
			}
		}

		virtual void Save(void* a_data) override
		{
			Impl::IniSettingSave<std::vector<T>>(a_data, m_section, m_key, this->m_value, m_delimiter);
		}

	private:
		std::string_view m_section;
		std::string_view m_key;
		std::string_view m_delimiter;
	};
}

namespace REX::INI
{
	template <class S = FIniSettingStore>
	using StrA = TIniSettingA<std::string, S>;
}

#endif

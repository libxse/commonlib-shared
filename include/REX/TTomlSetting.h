#pragma once

#ifdef COMMONLIB_OPTION_TOML

#	include "REX/FTomlSettingStore.h"
#	include "REX/TSetting.h"

namespace REX::Impl
{
	template <class T>
	void TomlSettingLoad(void* a_file, std::vector<std::string> a_section, std::string_view a_key, T& a_value, T& a_valueDefault);

	template <class T>
	void TomlSettingSave(void* a_file, std::vector<std::string> a_section, std::string_view a_key, T& a_value);
}

namespace REX
{
	template <class T, class S = FTomlSettingStore>
	class TTomlSetting :
		public TSetting<T, S>
	{
	public:
		TTomlSetting(std::string_view a_key, T a_default) :
			TSetting<T, S>(a_default),
			m_section(),
			m_key(a_key)
		{}

		TTomlSetting(std::string_view a_section, std::string_view a_key, T a_default) :
			TSetting<T, S>(a_default),
			m_section(),
			m_key(a_key)
		{
			for (const auto token : std::ranges::split_view{ a_section, std::string_view{ "." } }) {
				m_section.emplace_back(token.data(), token.size());
			}
		}

		TTomlSetting(std::initializer_list<std::string> a_section, std::string_view a_key, T a_default) :
			TSetting<T, S>(a_default),
			m_section(a_section),
			m_key(a_key)
		{}

	public:
		virtual void Load(void* a_data, bool a_isBase) override
		{
			if (a_isBase) {
				Impl::TomlSettingLoad(a_data, m_section, m_key, this->m_valueDefault, this->m_valueDefault);
				this->SetValue(this->m_valueDefault);
			} else {
				Impl::TomlSettingLoad(a_data, m_section, m_key, this->m_value, this->m_valueDefault);
			}
		}

		virtual void Save(void* a_data) override
		{
			Impl::TomlSettingSave(a_data, m_section, m_key, this->m_value);
		}

	private:
		std::vector<std::string> m_section;
		std::string_view         m_key;
	};
}

namespace REX::TOML
{
	template <class T, class S = FTomlSettingStore>
	using Setting [[deprecated("Renamed to 'REX::TTomlSetting")]] = TTomlSetting<T, S>;

	template <class S = FTomlSettingStore>
	using Bool = TTomlSetting<bool, S>;

	template <class S = FTomlSettingStore>
	using F32 = TTomlSetting<float, S>;

	template <class S = FTomlSettingStore>
	using F64 = TTomlSetting<double, S>;

	template <class S = FTomlSettingStore>
	using I8 = TTomlSetting<std::int8_t, S>;

	template <class S = FTomlSettingStore>
	using I16 = TTomlSetting<std::int16_t, S>;

	template <class S = FTomlSettingStore>
	using I32 = TTomlSetting<std::int32_t, S>;

	template <class S = FTomlSettingStore>
	using U8 = TTomlSetting<std::uint8_t, S>;

	template <class S = FTomlSettingStore>
	using U16 = TTomlSetting<std::uint16_t, S>;

	template <class S = FTomlSettingStore>
	using U32 = TTomlSetting<std::uint32_t, S>;

	template <class S = FTomlSettingStore>
	using Str = TTomlSetting<std::string, S>;
}

#endif

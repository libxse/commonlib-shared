#pragma once

#ifdef COMMONLIB_OPTION_TOML

#	include "REX/FTomlSettingStore.h"
#	include "REX/TSetting.h"

namespace REX::Impl
{
	template <class T>
	void TomlSettingLoad(void* a_data, std::vector<std::string> a_section, std::string_view a_key, T& a_value, T& a_valueDefault, std::vector<std::string>& a_description);

	template <class T>
	void TomlSettingSave(void* a_data, std::vector<std::string> a_section, std::string_view a_key, T& a_value, std::vector<std::string> a_description);
}

namespace REX
{
	template <class T, class S = FTomlSettingStore>
	class TTomlSetting :
		public TSetting<T>
	{
	public:
		TTomlSetting(std::string_view a_key, T a_default) :
			TSetting<T>(a_default),
			m_section(),
			m_key(a_key),
			m_description()
		{
			S::GetSingleton()->Add(this);
		}

		TTomlSetting(std::string_view a_section, std::string_view a_key, T a_default) :
			TSetting<T>(a_default),
			m_section(),
			m_key(a_key),
			m_description()
		{
			for (const auto token : std::ranges::split_view{ a_section, std::string_view{ "." } }) {
				m_section.emplace_back(token.data(), token.size());
			}

			S::GetSingleton()->Add(this);
		}

		TTomlSetting(std::initializer_list<std::string> a_section, std::string_view a_key, T a_default) :
			TSetting<T>(a_default),
			m_section(a_section),
			m_key(a_key),
			m_description()
		{
			S::GetSingleton()->Add(this);
		}

		TTomlSetting(std::string_view a_key, T a_default, std::string_view a_description) :
			TSetting<T>(a_default),
			m_section(),
			m_key(a_key),
			m_description()
		{
			m_description.emplace_back(a_description);
			S::GetSingleton()->Add(this);
		}

		TTomlSetting(std::string_view a_section, std::string_view a_key, T a_default, std::string_view a_description) :
			TSetting<T>(a_default),
			m_section(),
			m_key(a_key),
			m_description()
		{
			for (const auto token : std::ranges::split_view{ a_section, std::string_view{ "." } }) {
				m_section.emplace_back(token.data(), token.size());
			}

			m_description.emplace_back(a_description);
			S::GetSingleton()->Add(this);
		}

		TTomlSetting(std::initializer_list<std::string> a_section, std::string_view a_key, T a_default, std::string_view a_description) :
			TSetting<T>(a_default),
			m_section(a_section),
			m_key(a_key),
			m_description()
		{
			m_description.emplace_back(a_description);
			S::GetSingleton()->Add(this);
		}

		TTomlSetting(std::string_view a_key, T a_default, std::initializer_list<std::string> a_description) :
			TSetting<T>(a_default),
			m_section(),
			m_key(a_key),
			m_description(a_description)
		{
			S::GetSingleton()->Add(this);
		}

		TTomlSetting(std::string_view a_section, std::string_view a_key, T a_default, std::initializer_list<std::string> a_description) :
			TSetting<T>(a_default),
			m_section(),
			m_key(a_key),
			m_description(a_description)
		{
			for (const auto token : std::ranges::split_view{ a_section, std::string_view{ "." } }) {
				m_section.emplace_back(token.data(), token.size());
			}

			S::GetSingleton()->Add(this);
		}

		TTomlSetting(std::initializer_list<std::string> a_section, std::string_view a_key, T a_default, std::initializer_list<std::string> a_description) :
			TSetting<T>(a_default),
			m_section(a_section),
			m_key(a_key),
			m_description(a_description)
		{
			S::GetSingleton()->Add(this);
		}

	public:
		virtual void Load(void* a_data, bool a_isBase) override
		{
			if (a_isBase) {
				Impl::TomlSettingLoad<T>(a_data, m_section, m_key, this->m_valueDefault, this->m_valueDefault, m_description);
				this->SetValue(this->m_valueDefault);
			} else {
				Impl::TomlSettingLoad<T>(a_data, m_section, m_key, this->m_value, this->m_valueDefault, m_description);
			}
		}

		virtual void Save(void* a_data) override
		{
			Impl::TomlSettingSave<T>(a_data, m_section, m_key, this->m_value, m_description);
		}

		std::vector<std::string> GetDescription() const { return m_description; }

	private:
		std::vector<std::string> m_section;
		std::string_view         m_key;
		std::vector<std::string> m_description;
	};
}

// DEPRECATED
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

#include "REX/REX/JSON.h"

#ifdef COMMONLIB_OPTION_JSON
#	include <glaze/glaze.hpp>

namespace REX::JSON
{
	namespace Impl
	{
		template <class T>
		void SettingLoad<T>(
			void*  a_data,
			path_t a_path,
			T&     a_value,
			T&     a_valueDefault)
		{
			const auto& json = *static_cast<glz::generic*>(a_data);
			if (a_path[0] != '/') {
				const auto path = std::format("/{}"sv, a_path);
				a_value = glz::get<T>(json, path).value_or(a_valueDefault);
			} else {
				a_value = glz::get<T>(json, a_path).value_or(a_valueDefault);
			}
		}

		template void SettingLoad<bool>(void*, path_t, bool&, bool&);
		template void SettingLoad<float>(void*, path_t, float&, float&);
		template void SettingLoad<double>(void*, path_t, double&, double&);
		template void SettingLoad<std::uint8_t>(void*, path_t, std::uint8_t&, std::uint8_t&);
		template void SettingLoad<std::uint16_t>(void*, path_t, std::uint16_t&, std::uint16_t&);
		template void SettingLoad<std::uint32_t>(void*, path_t, std::uint32_t&, std::uint32_t&);
		template void SettingLoad<std::int8_t>(void*, path_t, std::int8_t&, std::int8_t&);
		template void SettingLoad<std::int16_t>(void*, path_t, std::int16_t&, std::int16_t&);
		template void SettingLoad<std::int32_t>(void*, path_t, std::int32_t&, std::int32_t&);
		template void SettingLoad<std::string>(void*, path_t, std::string&, std::string&);
		template void SettingLoad<std::vector<bool>>(void*, path_t, std::vector<bool>&, std::vector<bool>&);
		template void SettingLoad<std::vector<float>>(void*, path_t, std::vector<float>&, std::vector<float>&);
		template void SettingLoad<std::vector<double>>(void*, path_t, std::vector<double>&, std::vector<double>&);
		template void SettingLoad<std::vector<std::uint8_t>>(void*, path_t, std::vector<std::uint8_t>&, std::vector<std::uint8_t>&);
		template void SettingLoad<std::vector<std::uint16_t>>(void*, path_t, std::vector<std::uint16_t>&, std::vector<std::uint16_t>&);
		template void SettingLoad<std::vector<std::uint32_t>>(void*, path_t, std::vector<std::uint32_t>&, std::vector<std::uint32_t>&);
		template void SettingLoad<std::vector<std::int8_t>>(void*, path_t, std::vector<std::int8_t>&, std::vector<std::int8_t>&);
		template void SettingLoad<std::vector<std::int16_t>>(void*, path_t, std::vector<std::int16_t>&, std::vector<std::int16_t>&);
		template void SettingLoad<std::vector<std::int32_t>>(void*, path_t, std::vector<std::int32_t>&, std::vector<std::int32_t>&);
		template void SettingLoad<std::vector<std::string>>(void*, path_t, std::vector<std::string>&, std::vector<std::string>&);

		template <class T>
		void SettingSave<T>(
			void*  a_data,
			path_t a_path,
			T&     a_value)
		{
			auto& json = *static_cast<glz::generic*>(a_data);
			if (a_path[0] != '/') {
				const auto path = std::format("/{}"sv, a_path);
				glz::set(json, path, a_value);
			} else {
				glz::set(json, a_path, a_value);
			}
		}

		template void SettingSave<bool>(void*, path_t, bool&);
		template void SettingSave<float>(void*, path_t, float&);
		template void SettingSave<double>(void*, path_t, double&);
		template void SettingSave<std::uint8_t>(void*, path_t, std::uint8_t&);
		template void SettingSave<std::uint16_t>(void*, path_t, std::uint16_t&);
		template void SettingSave<std::uint32_t>(void*, path_t, std::uint32_t&);
		template void SettingSave<std::int8_t>(void*, path_t, std::int8_t&);
		template void SettingSave<std::int16_t>(void*, path_t, std::int16_t&);
		template void SettingSave<std::int32_t>(void*, path_t, std::int32_t&);
		template void SettingSave<std::string>(void*, path_t, std::string&);
		template void SettingSave<std::vector<bool>>(void*, path_t, std::vector<bool>&);
		template void SettingSave<std::vector<float>>(void*, path_t, std::vector<float>&);
		template void SettingSave<std::vector<double>>(void*, path_t, std::vector<double>&);
		template void SettingSave<std::vector<std::uint8_t>>(void*, path_t, std::vector<std::uint8_t>&);
		template void SettingSave<std::vector<std::uint16_t>>(void*, path_t, std::vector<std::uint16_t>&);
		template void SettingSave<std::vector<std::uint32_t>>(void*, path_t, std::vector<std::uint32_t>&);
		template void SettingSave<std::vector<std::int8_t>>(void*, path_t, std::vector<std::int8_t>&);
		template void SettingSave<std::vector<std::int16_t>>(void*, path_t, std::vector<std::int16_t>&);
		template void SettingSave<std::vector<std::int32_t>>(void*, path_t, std::vector<std::int32_t>&);
		template void SettingSave<std::vector<std::string>>(void*, path_t, std::vector<std::string>&);
	}

	void SettingStore::Load()
	{
		if (std::filesystem::exists(m_fileBase)) {
			glz::generic result{};
			if (!glz::read_file_json(result, m_fileBase, std::string{})) {
				for (auto setting : m_settings) {
					setting->Load(&result, true);
				}
			}
		}

		if (std::filesystem::exists(m_fileUser)) {
			glz::generic result{};
			if (!glz::read_file_json(result, m_fileUser, std::string{})) {
				for (auto setting : m_settings) {
					setting->Load(&result, false);
				}
			}
		}
	}

	void SettingStore::Save()
	{
		glz::generic output{};
		if (std::filesystem::exists(m_fileBase)) {
			(void)glz::read_file_json(output, m_fileBase, std::string{});
		}

		for (auto& setting : m_settings) {
			setting->Save(&output);
		}

		constexpr glz::opts opts{ .prettify = true, .indentation_width = 4 };
		(void)glz::write_file_json<opts>(output, m_fileBase, std::string{});
	}
}
#endif

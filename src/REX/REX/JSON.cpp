#include "REX/REX/JSON.h"

#include "REX/REX/LOG.h"

#ifdef COMMONLIB_OPTION_JSON
#	include <nlohmann/json.hpp>

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
			const auto& json = *static_cast<nlohmann::json*>(a_data);
			if (a_path[0] == '/') {
				a_value = json.value<T>(nlohmann::json::json_pointer(a_path.data()), a_valueDefault);
			} else {
				a_value = json.value<T>(a_path, a_valueDefault);
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

		template <class T>
		void SettingSave<T>(
			void*  a_data,
			path_t a_path,
			T&     a_value)
		{
			auto& json = *static_cast<nlohmann::json*>(a_data);
			if (a_path[0] == '/') {
				json[nlohmann::json::json_pointer(a_path.data())] = a_value;
			} else {
				json[a_path] = a_value;
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
	}

	void SettingStore::Load()
	{
		if (std::filesystem::exists(m_fileBase)) {
			std::ifstream file{ m_fileBase.data() };
			try {
				auto result = nlohmann::json::parse(file);
				for (auto setting : m_settings) {
					setting->Load(&result, true);
				}
			} catch (const std::exception& e) {
				REX::ERROR("{}", e.what());
			}
		}

		if (std::filesystem::exists(m_fileUser)) {
			std::ifstream file{ m_fileUser.data() };
			try {
				auto result = nlohmann::json::parse(file);
				for (auto setting : m_settings) {
					setting->Load(&result, false);
				}
			} catch (const std::exception& e) {
				REX::ERROR("{}", e.what());
			}
		}
	}

	void SettingStore::Save()
	{
		nlohmann::json output{};
		if (std::filesystem::exists(m_fileBase)) {
			std::ifstream file{ m_fileBase.data() };
			output = nlohmann::json::parse(file);
		}

		for (auto& setting : m_settings) {
			setting->Save(&output);
		}

		std::ofstream file{ m_fileBase.data(), std::ios::trunc };
		file << std::setw(4) << output;
	}
}
#endif

#include "REX/REX/INI.h"
#include "REX/REX/JSON.h"
#include "REX/REX/LOG.h"
#include "REX/REX/TOML.h"

#include "REX/W32/KERNEL32.h"
#include "REX/W32/USER32.h"

#include <spdlog/spdlog.h>

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

#ifdef COMMONLIB_OPTION_TOML
#	include <toml.hpp>

namespace REX::TOML
{
	namespace Impl
	{
		static toml::value* recurse_table(toml::value* a_result, toml::value& a_value, const std::string& a_section, bool a_create)
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

		static bool recurse_mark_implicit(toml::value& a_value)
		{
			for (auto& kv : a_value.as_table()) {
				if (kv.second.is_table()) {
					if (!recurse_mark_implicit(kv.second)) {
						continue;
					}
					kv.second.as_table_fmt().fmt = toml::table_format::implicit;
				} else {
					return false;
				}
			}
			return true;
		}

		template <class T>
		void SettingLoad<T>(
			void* a_data,
			sec_t a_section,
			key_t a_key,
			T&    a_value,
			T&    a_valueDefault)
		{
			const auto& data = static_cast<toml::value*>(a_data);
			if (a_section.empty()) {
				auto& path = (*data);
				a_value = toml::find_or<T>(path, a_key.data(), a_valueDefault);
				return;
			} else if (a_section.size() == 1) {
				auto& path = (*data)[a_section.front()];
				a_value = toml::find_or<T>(path, a_key.data(), a_valueDefault);
				return;
			} else {
				toml::value* path{ nullptr };
				for (auto& section : a_section) {
					path = recurse_table(path, *data, section, false);
				}
				if (path) {
					a_value = toml::find_or<T>(*path, a_key.data(), a_valueDefault);
					return;
				}
			}
			a_value = a_valueDefault;
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
			auto& data = *static_cast<toml::value*>(a_data);
			if (a_section.empty()) {
				data[a_key.data()] = a_value;
			} else if (a_section.size() == 1) {
				data[a_section.front()][a_key.data()] = a_value;
			} else {
				toml::value* path{ nullptr };
				for (auto& section : a_section) {
					path = recurse_table(path, data, section, true);
				}
				if (path) {
					(*path)[a_key.data()] = a_value;
				}
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
		if (auto result = toml::try_parse(m_fileBase.data()); result.is_ok()) {
			for (auto& setting : m_settings) {
				setting->Load(&result.unwrap(), true);
			}
		}

		if (auto result = toml::try_parse(m_fileUser.data()); result.is_ok()) {
			for (auto& setting : m_settings) {
				setting->Load(&result.unwrap(), false);
			}
		}
	}

	void SettingStore::Save()
	{
		toml::value output{};
		if (auto result = toml::try_parse(m_fileBase.data()); result.is_ok()) {
			output = result.unwrap();
		}

		for (auto setting : m_settings) {
			setting->Save(&output);
		}

		Impl::recurse_mark_implicit(output);
		std::ofstream file{ m_fileBase.data(), std::ios::trunc };
		file << toml::format(output);
	}
}
#endif

namespace REX
{
	void LOG(const std::source_location a_loc, const LOG_LEVEL a_level, const std::string_view a_fmt)
	{
		const auto loc = spdlog::source_loc{ a_loc.file_name(), static_cast<std::int32_t>(a_loc.line()), a_loc.function_name() };
		switch (a_level) {
			case LOG_LEVEL::TRACE:
				spdlog::default_logger_raw()->log(loc, spdlog::level::trace, a_fmt);
				break;
			case LOG_LEVEL::DEBUG:
				spdlog::default_logger_raw()->log(loc, spdlog::level::debug, a_fmt);
				break;
			case LOG_LEVEL::INFO:
				spdlog::default_logger_raw()->log(loc, spdlog::level::info, a_fmt);
				break;
			case LOG_LEVEL::WARN:
				spdlog::default_logger_raw()->log(loc, spdlog::level::warn, a_fmt);
				break;
			case LOG_LEVEL::ERROR:
				spdlog::default_logger_raw()->log(loc, spdlog::level::err, a_fmt);
				break;
			case LOG_LEVEL::CRITICAL:
				spdlog::default_logger_raw()->log(loc, spdlog::level::critical, a_fmt);
				break;
		}
	}

	void LOG(const std::source_location a_loc, const LOG_LEVEL a_level, const std::wstring_view a_fmt)
	{
		const auto loc = spdlog::source_loc{ a_loc.file_name(), static_cast<std::int32_t>(a_loc.line()), a_loc.function_name() };
		switch (a_level) {
			case LOG_LEVEL::TRACE:
				spdlog::default_logger_raw()->log(loc, spdlog::level::trace, a_fmt);
				break;
			case LOG_LEVEL::DEBUG:
				spdlog::default_logger_raw()->log(loc, spdlog::level::debug, a_fmt);
				break;
			case LOG_LEVEL::INFO:
				spdlog::default_logger_raw()->log(loc, spdlog::level::info, a_fmt);
				break;
			case LOG_LEVEL::WARN:
				spdlog::default_logger_raw()->log(loc, spdlog::level::warn, a_fmt);
				break;
			case LOG_LEVEL::ERROR:
				spdlog::default_logger_raw()->log(loc, spdlog::level::err, a_fmt);
				break;
			case LOG_LEVEL::CRITICAL:
				spdlog::default_logger_raw()->log(loc, spdlog::level::critical, a_fmt);
				break;
		}
	}
}

namespace REX::IMPL
{
	[[noreturn]] void FAIL(const std::source_location a_loc, const std::string_view a_fmt)
	{
		const auto body = [&]() {
			constexpr std::array directories{
				"include/"sv,
				"src/"sv,
			};

			const std::filesystem::path p = a_loc.file_name();
			const auto                  filename = p.generic_string();
			std::string_view            fileview = filename;

			constexpr auto npos = std::string::npos;
			std::size_t    pos = npos;
			std::size_t    off = 0;
			for (const auto& dir : directories) {
				pos = fileview.find(dir);
				if (pos != npos) {
					off = dir.length();
					break;
				}
			}

			if (pos != npos) {
				fileview = fileview.substr(pos + off);
			}

			return std::format("{}({}): {}", fileview, a_loc.line(), a_fmt);
		}();

		const auto caption = []() -> std::string {
			std::vector<char> buf;
			buf.reserve(REX::W32::MAX_PATH);
			buf.resize(REX::W32::MAX_PATH / 2);
			std::uint32_t result = 0;
			do {
				buf.resize(buf.size() * 2);
				result = REX::W32::GetModuleFileNameA(
					REX::W32::GetCurrentModule(),
					buf.data(),
					static_cast<std::uint32_t>(buf.size()));
			} while (result && result == buf.size() && buf.size() <= std::numeric_limits<std::uint32_t>::max());

			if (result && result != buf.size()) {
				std::filesystem::path p(buf.begin(), buf.begin() + result);
				return p.filename().string();
			} else {
				return {};
			}
		}();

		LOG(a_loc, LOG_LEVEL::CRITICAL, a_fmt);
		REX::W32::MessageBoxA(nullptr, body.c_str(), (caption.empty() ? nullptr : caption.c_str()), 0);
		REX::W32::TerminateProcess(REX::W32::GetCurrentProcess(), 1);
	}

	[[noreturn]] void FAIL(const std::source_location a_loc, const std::wstring_view a_fmt)
	{
		const auto body = [&]() {
			constexpr std::array directories{
				L"include/"sv,
				L"src/"sv,
			};

			const std::filesystem::path p = a_loc.file_name();
			const auto                  filename = p.generic_wstring();
			std::wstring_view           fileview = filename;

			constexpr auto npos = std::wstring::npos;
			std::size_t    pos = npos;
			std::size_t    off = 0;
			for (const auto& dir : directories) {
				pos = fileview.find(dir);
				if (pos != npos) {
					off = dir.length();
					break;
				}
			}

			if (pos != npos) {
				fileview = fileview.substr(pos + off);
			}

			return std::format(L"{}({}): {}", fileview, a_loc.line(), a_fmt);
		}();

		const auto caption = []() -> std::wstring {
			std::vector<wchar_t> buf;
			buf.reserve(REX::W32::MAX_PATH);
			buf.resize(REX::W32::MAX_PATH / 2);
			std::uint32_t result = 0;
			do {
				buf.resize(buf.size() * 2);
				result = REX::W32::GetModuleFileNameW(
					REX::W32::GetCurrentModule(),
					buf.data(),
					static_cast<std::uint32_t>(buf.size()));
			} while (result && result == buf.size() && buf.size() <= std::numeric_limits<std::uint32_t>::max());

			if (result && result != buf.size()) {
				std::filesystem::path p(buf.begin(), buf.begin() + result);
				return p.filename().wstring();
			} else {
				return {};
			}
		}();

		LOG(a_loc, LOG_LEVEL::CRITICAL, a_fmt);
		REX::W32::MessageBoxW(nullptr, body.c_str(), (caption.empty() ? nullptr : caption.c_str()), 0);
		REX::W32::TerminateProcess(REX::W32::GetCurrentProcess(), 1);
	}
}

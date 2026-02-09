#pragma once

#include "REX/ISettingStore.h"
#include "REX/TSingleton.h"

namespace REX
{
	template <class T>
	class TSettingStore :
		public ISettingStore,
		public TSingleton<T>
	{
	public:
		virtual void Init(const char* a_fileBase, const char* a_fileUser) override
		{
			m_fileBase = a_fileBase;
			m_fileUser = a_fileUser;
		}

		virtual void Register(ISetting* a_setting) override
		{
			m_settings.emplace_back(a_setting);
		}

	protected:
		std::string_view       m_fileBase;
		std::string_view       m_fileUser;
		std::vector<ISetting*> m_settings;
	};
}

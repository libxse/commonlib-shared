#pragma once

#ifdef COMMONLIB_OPTION_INI

#include "REX/TSettingStore.h"

namespace REX
{
	class FIniSettingStore :
		public TSettingStore<FIniSettingStore>
	{
	public:
		virtual void Load() override;
		virtual void Save() override;
	};
}

namespace REX::INI
{
	using SettingStore [[deprecated("Renamed to 'REX::FIniSettingStore'")]] = FIniSettingStore;
}

#endif

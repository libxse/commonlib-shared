#pragma once

#ifdef COMMONLIB_OPTION_TOML

#include "REX/TSettingStore.h"

namespace REX
{
	class FTomlSettingStore :
		public TSettingStore<FTomlSettingStore>
	{
	public:
		virtual void Load() override;
		virtual void Save() override;
	};
}

namespace REX::TOML
{
	using SettingStore [[deprecated("Renamed to 'REX::FTomlSettingStore")]] = FTomlSettingStore;
}

#endif

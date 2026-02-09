#pragma once

#ifdef COMMONLIB_OPTION_JSON

#include "REX/TSettingStore.h"

namespace REX
{
    class FJsonSettingStore :
		public TSettingStore<FJsonSettingStore>
	{
	public:
		virtual void Load() override;
		virtual void Save() override;
	};
}

namespace REX::JSON
{
	using SettingStore [[deprecated("Renamed to 'REX::FJsonSettingStore")]] = FJsonSettingStore;
}

#endif

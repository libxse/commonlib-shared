#pragma once

#include "REX/W32/BASE.h"

namespace REX::W32
{
	inline constexpr auto XAUDIO2FX_REVERB_MIN_WET_DRY_MIX{ 0.0f };
	inline constexpr auto XAUDIO2FX_REVERB_MIN_REFLECTIONS_DELAY{ 0 };
	inline constexpr auto XAUDIO2FX_REVERB_MIN_REVERB_DELAY{ 0 };
	inline constexpr auto XAUDIO2FX_REVERB_MIN_REAR_DELAY{ 0 };
	inline constexpr auto XAUDIO2FX_REVERB_MIN_POSITION{ 0 };
	inline constexpr auto XAUDIO2FX_REVERB_MIN_DIFFUSION{ 0 };
	inline constexpr auto XAUDIO2FX_REVERB_MIN_LOW_EQ_GAIN{ 0 };
	inline constexpr auto XAUDIO2FX_REVERB_MIN_LOW_EQ_CUTOFF{ 0 };
	inline constexpr auto XAUDIO2FX_REVERB_MIN_HIGH_EQ_GAIN{ 0 };
	inline constexpr auto XAUDIO2FX_REVERB_MIN_HIGH_EQ_CUTOFF{ 0 };
	inline constexpr auto XAUDIO2FX_REVERB_MIN_ROOM_FILTER_FREQ{ 20.0f };
	inline constexpr auto XAUDIO2FX_REVERB_MIN_ROOM_FILTER_MAIN{ -100.0f };
	inline constexpr auto XAUDIO2FX_REVERB_MIN_ROOM_FILTER_HF{ -100.0f };
	inline constexpr auto XAUDIO2FX_REVERB_MIN_REFLECTIONS_GAIN{ -100.0f };
	inline constexpr auto XAUDIO2FX_REVERB_MIN_REVERB_GAIN{ -100.0f };
	inline constexpr auto XAUDIO2FX_REVERB_MIN_DECAY_TIME{ 0.1f };
	inline constexpr auto XAUDIO2FX_REVERB_MIN_DENSITY{ 0.0f };
	inline constexpr auto XAUDIO2FX_REVERB_MIN_ROOM_SIZE{ 0.0f };

	inline constexpr auto XAUDIO2FX_REVERB_MAX_WET_DRY_MIX{ 100.0f };
	inline constexpr auto XAUDIO2FX_REVERB_MAX_REFLECTIONS_DELAY{ 300 };
	inline constexpr auto XAUDIO2FX_REVERB_MAX_REVERB_DELAY{ 85 };
	inline constexpr auto XAUDIO2FX_REVERB_MAX_REAR_DELAY{ 5 };
	inline constexpr auto XAUDIO2FX_REVERB_MAX_POSITION{ 30 };
	inline constexpr auto XAUDIO2FX_REVERB_MAX_DIFFUSION{ 15 };
	inline constexpr auto XAUDIO2FX_REVERB_MAX_LOW_EQ_GAIN{ 12 };
	inline constexpr auto XAUDIO2FX_REVERB_MAX_LOW_EQ_CUTOFF{ 9 };
	inline constexpr auto XAUDIO2FX_REVERB_MAX_HIGH_EQ_GAIN{ 8 };
	inline constexpr auto XAUDIO2FX_REVERB_MAX_HIGH_EQ_CUTOFF{ 14 };
	inline constexpr auto XAUDIO2FX_REVERB_MAX_ROOM_FILTER_FREQ{ 20000.0f };
	inline constexpr auto XAUDIO2FX_REVERB_MAX_ROOM_FILTER_MAIN{ 0.0f };
	inline constexpr auto XAUDIO2FX_REVERB_MAX_ROOM_FILTER_HF{ 0.0f };
	inline constexpr auto XAUDIO2FX_REVERB_MAX_REFLECTIONS_GAIN{ 20.0f };
	inline constexpr auto XAUDIO2FX_REVERB_MAX_REVERB_GAIN{ 20.0f };
	inline constexpr auto XAUDIO2FX_REVERB_MAX_DENSITY{ 100.0f };
	inline constexpr auto XAUDIO2FX_REVERB_MAX_ROOM_SIZE{ 100.0f };

	inline constexpr auto XAUDIO2FX_REVERB_DEFAULT_WET_DRY_MIX{ 100.0f };
	inline constexpr auto XAUDIO2FX_REVERB_DEFAULT_REFLECTIONS_DELAY{ 5 };
	inline constexpr auto XAUDIO2FX_REVERB_DEFAULT_REVERB_DELAY{ 5 };
	inline constexpr auto XAUDIO2FX_REVERB_DEFAULT_REAR_DELAY{ 5 };
	inline constexpr auto XAUDIO2FX_REVERB_DEFAULT_POSITION{ 6 };
	inline constexpr auto XAUDIO2FX_REVERB_DEFAULT_POSITION_MATRIX{ 27 };
	inline constexpr auto XAUDIO2FX_REVERB_DEFAULT_EARLY_DIFFUSION{ 8 };
	inline constexpr auto XAUDIO2FX_REVERB_DEFAULT_LATE_DIFFUSION{ 8 };
	inline constexpr auto XAUDIO2FX_REVERB_DEFAULT_LOW_EQ_GAIN{ 8 };
	inline constexpr auto XAUDIO2FX_REVERB_DEFAULT_LOW_EQ_CUTOFF{ 4 };
	inline constexpr auto XAUDIO2FX_REVERB_DEFAULT_HIGH_EQ_GAIN{ 8 };
	inline constexpr auto XAUDIO2FX_REVERB_DEFAULT_HIGH_EQ_CUTOFF{ 4 };
	inline constexpr auto XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_FREQ{ 5000.0f };
	inline constexpr auto XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_MAIN{ 0.0f };
	inline constexpr auto XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_HF{ 0.0f };
	inline constexpr auto XAUDIO2FX_REVERB_DEFAULT_REFLECTIONS_GAIN{ 0.0f };
	inline constexpr auto XAUDIO2FX_REVERB_DEFAULT_REVERB_GAIN{ 0.0f };
	inline constexpr auto XAUDIO2FX_REVERB_DEFAULT_DECAY_TIME{ 1.0f };
	inline constexpr auto XAUDIO2FX_REVERB_DEFAULT_DENSITY{ 100.0f };
	inline constexpr auto XAUDIO2FX_REVERB_DEFAULT_ROOM_SIZE{ 100.0f };
}

#pragma pack(push, 1)

namespace REX::W32
{
	struct XAUDIO2FX_REVERB_I3DL2_PARAMETERS
	{
		float        wetDryMix;
		std::int32_t room;
		std::int32_t roomHF;
		float        roomRolloffFactor;
		float        decayTime;
		float        decayHFRatio;
		std::int32_t reflections;
		float        reflectionsDelay;
		std::int32_t reverb;
		float        reverbDelay;
		float        diffusion;
		float        density;
		float        hfReference;
	};

	struct XAUDIO2FX_REVERB_PARAMETERS
	{
		float         wetDryMix;
		std::uint32_t reflectionsDelay;
		std::uint8_t  reverbDelay;
		std::uint8_t  rearDelay;
		std::uint8_t  positionLeft;
		std::uint8_t  positionRight;
		std::uint8_t  positionMatrixLeft;
		std::uint8_t  positionMatrixRight;
		std::uint8_t  earlyDiffusion;
		std::uint8_t  lateDiffusion;
		std::uint8_t  lowEQGain;
		std::uint8_t  lowEQCutoff;
		std::uint8_t  highEQGain;
		std::uint8_t  highEQCutoff;
		float         roomFilterFreq;
		float         roomFilterMain;
		float         roomFilterHF;
		float         reflectionsGain;
		float         reverbGain;
		float         decayTime;
		float         density;
		float         roomSize;
	};
}

#pragma pack(pop)

namespace REX::W32
{
	inline constexpr XAUDIO2FX_REVERB_I3DL2_PARAMETERS XAUDIO2FX_I3DL2_PRESET_DEFAULT{ 100, -10000, 0, .0f, 1.f, .5f, -10000, .02f, -10000, .04f, 100.f, 100.f, 5000.f };
}

#pragma once

#include "REX/W32/D3D.h"

namespace REX::W32
{
	inline constexpr auto X3DAUDIO_HANDLE_BYTESIZE{ 20 };

	inline constexpr auto X3DAUDIO_SPEED_OF_SOUND{ 343.5f };

	inline constexpr auto X3DAUDIO_CALCULATE_MATRIX{ 0x00000001 };
	inline constexpr auto X3DAUDIO_CALCULATE_DELAY{ 0x00000002 };
	inline constexpr auto X3DAUDIO_CALCULATE_LPF_DIRECT{ 0x00000004 };
	inline constexpr auto X3DAUDIO_CALCULATE_LPF_REVERB{ 0x00000008 };
	inline constexpr auto X3DAUDIO_CALCULATE_REVERB{ 0x00000010 };
	inline constexpr auto X3DAUDIO_CALCULATE_DOPPLER{ 0x00000020 };
	inline constexpr auto X3DAUDIO_CALCULATE_EMITTER_ANGLE{ 0x00000040 };
	inline constexpr auto X3DAUDIO_CALCULATE_ZEROCENTER{ 0x00010000 };
	inline constexpr auto X3DAUDIO_CALCULATE_REDIRECT_TO_LFE{ 0x00020000 };
}

#pragma pack(push, 1)

namespace REX::W32
{
	using X3DAUDIO_HANDLE = std::uint8_t[X3DAUDIO_HANDLE_BYTESIZE];
	using X3DAUDIO_VECTOR = XMFLOAT3;

	struct X3DAUDIO_CONE
	{
		float innerAngle;
		float outerAngle;
		float innerVolume;
		float outerVolume;
		float innerLPF;
		float outerLPF;
		float innerReverb;
		float outerReverb;
	};

	struct X3DAUDIO_DISTANCE_CURVE_POINT
	{
		float distance;
		float dspSetting;
	};

	struct X3DAUDIO_DISTANCE_CURVE
	{
		X3DAUDIO_DISTANCE_CURVE_POINT* points;
		std::uint32_t                  pointCount;
	};

	struct X3DAUDIO_DSP_SETTINGS
	{
		float*        matrixCoefficients;
		float*        delayTimes;
		std::uint32_t srcChannelCount;
		std::uint32_t dstChannelCount;
		float         lpfDirectCoefficient;
		float         lpfReverbCoefficient;
		float         reverbLevel;
		float         dopplerFactor;
		float         emitterToListenerAngle;
		float         emitterToListenerDistance;
		float         emitterVelocityComponent;
		float         listenerVelocityComponent;
	};

	struct X3DAUDIO_EMITTER
	{
		X3DAUDIO_CONE*           cone;
		X3DAUDIO_VECTOR          orientFront;
		X3DAUDIO_VECTOR          orientTop;
		X3DAUDIO_VECTOR          position;
		X3DAUDIO_VECTOR          velocity;
		float                    innerRadius;
		float                    innerRadiusAngle;
		std::uint32_t            channelCount;
		float                    channelRadius;
		float*                   channelAzimuths;
		X3DAUDIO_DISTANCE_CURVE* volumeCurve;
		X3DAUDIO_DISTANCE_CURVE* lfeCurve;
		X3DAUDIO_DISTANCE_CURVE* lpfDirectCurve;
		X3DAUDIO_DISTANCE_CURVE* lpfReverbCurve;
		X3DAUDIO_DISTANCE_CURVE* reverbCurve;
		float                    curveDistanceScaler;
		float                    dopplerScaler;
	};

	struct X3DAUDIO_LISTENER
	{
		X3DAUDIO_VECTOR orientFront;
		X3DAUDIO_VECTOR orientTop;
		X3DAUDIO_VECTOR position;
		X3DAUDIO_VECTOR velocity;
		X3DAUDIO_CONE*  cone;
	};
}

#pragma pack(pop)

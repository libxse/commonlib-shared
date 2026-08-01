#pragma once

#include "REX/W32/BASE.h"

namespace REX::W32
{
	struct IXAudio2EngineCallback;
	struct IXAudio2Extension;
	struct IXAudio2VoiceCallback;
	struct IXAudio27;
	struct IXAudio27Voice;
	struct IXAudio27SourceVoice;
	struct IXAudio27SubmixVoice;
	struct IXAudio27MasteringVoice;
}

namespace REX::W32
{
	inline constexpr auto XAUDIO2_MAX_BUFFER_BYTES{ 0x80000000 };
	inline constexpr auto XAUDIO2_MAX_QUEUED_BUFFERS{ 64 };
	inline constexpr auto XAUDIO2_MAX_BUFFERS_SYSTEM{ 2 };
	inline constexpr auto XAUDIO2_MAX_AUDIO_CHANNELS{ 64 };
	inline constexpr auto XAUDIO2_MIN_SAMPLE_RATE{ 1000 };
	inline constexpr auto XAUDIO2_MAX_SAMPLE_RATE{ 200000 };
	inline constexpr auto XAUDIO2_MAX_VOLUME_LEVEL{ 16777216.0f };
	inline constexpr auto XAUDIO2_MIN_FREQ_RATIO{ 1.0f / 1024.0f };
	inline constexpr auto XAUDIO2_MAX_FREQ_RATIO{ 1024.0f };
	inline constexpr auto XAUDIO2_DEFAULT_FREQ_RATIO{ 2.0f };
	inline constexpr auto XAUDIO2_MAX_FILTER_ONEOVERQ{ 1.5f };
	inline constexpr auto XAUDIO2_MAX_FILTER_FREQUENCY{ 1.0f };
	inline constexpr auto XAUDIO2_MAX_LOOP_COUNT{ 254 };
	inline constexpr auto XAUDIO2_MAX_INSTANCES{ 8 };

	inline constexpr auto XAUDIO2_MAX_RATIO_TIMES_RATE_XMA_MONO{ 600000 };
	inline constexpr auto XAUDIO2_MAX_RATIO_TIMES_RATE_XMA_MULTICHANNEL{ 300000 };

	inline constexpr auto XAUDIO2_COMMIT_NOW{ 0 };
	inline constexpr auto XAUDIO2_COMMIT_ALL{ 0 };
	inline constexpr auto XAUDIO2_INVALID_OPSET{ static_cast<std::uint32_t>(-1) };
	inline constexpr auto XAUDIO2_NO_LOOP_REGION{ 0 };
	inline constexpr auto XAUDIO2_LOOP_INFINITE{ 255 };
	inline constexpr auto XAUDIO2_DEFAULT_CHANNELS{ 0 };
	inline constexpr auto XAUDIO2_DEFAULT_SAMPLERATE{ 0 };

	inline constexpr auto XAUDIO2_DEBUG_ENGINE{ 0x0001 };
	inline constexpr auto XAUDIO2_VOICE_NOPITCH{ 0x0002 };
	inline constexpr auto XAUDIO2_VOICE_NOSRC{ 0x0004 };
	inline constexpr auto XAUDIO2_VOICE_USEFILTER{ 0x0008 };
	inline constexpr auto XAUDIO2_VOICE_MUSIC{ 0x0010 };
	inline constexpr auto XAUDIO2_PLAY_TAILS{ 0x0020 };
	inline constexpr auto XAUDIO2_END_OF_STREAM{ 0x0040 };
	inline constexpr auto XAUDIO2_SEND_USEFILTER{ 0x0080 };

	inline constexpr auto XAUDIO2_DEFAULT_FILTER_TYPE{ 0 };
	inline constexpr auto XAUDIO2_DEFAULT_FILTER_FREQUENCY{ XAUDIO2_MAX_FILTER_FREQUENCY };
	inline constexpr auto XAUDIO2_DEFAULT_FILTER_ONEOVERQ{ 1.0f };
}

namespace REX::W32
{
	enum XAUDIO2_FILTER_TYPE
	{
		LowPassFilter,
		BandPassFilter,
		HighPassFilter,
		NotchFilter
	};

	enum XAUDIO2_DEVICE_ROLE
	{
		NotDefaultDevice = 0x0,
		DefaultConsoleDevice = 0x1,
		DefaultMultimediaDevice = 0x2,
		DefaultCommunicationsDevice = 0x4,
		DefaultGameDevice = 0x8,
		GlobalDefaultDevice = 0xf,
		InvalidDeviceRole = ~GlobalDefaultDevice
	};

	enum XAUDIO2_PROCESSOR
	{
		Processor1 = 0x00000001,
		Processor2 = 0x00000002,
		Processor3 = 0x00000004,
		Processor4 = 0x00000008,
		Processor5 = 0x00000010,
		Processor6 = 0x00000020,
		Processor7 = 0x00000040,
		Processor8 = 0x00000080,
		Processor9 = 0x00000100,
		Processor10 = 0x00000200,
		Processor11 = 0x00000400,
		Processor12 = 0x00000800,
		Processor13 = 0x00001000,
		Processor14 = 0x00002000,
		Processor15 = 0x00004000,
		Processor16 = 0x00008000,
		Processor17 = 0x00010000,
		Processor18 = 0x00020000,
		Processor19 = 0x00040000,
		Processor20 = 0x00080000,
		Processor21 = 0x00100000,
		Processor22 = 0x00200000,
		Processor23 = 0x00400000,
		Processor24 = 0x00800000,
		Processor25 = 0x01000000,
		Processor26 = 0x02000000,
		Processor27 = 0x04000000,
		Processor28 = 0x08000000,
		Processor29 = 0x10000000,
		Processor30 = 0x20000000,
		Processor31 = 0x40000000,
		Processor32 = 0x80000000,
		XAUDIO2_ANY_PROCESSOR = 0xffffffff,
		XAUDIO2_DEFAULT_PROCESSOR = XAUDIO2_ANY_PROCESSOR
	};

	using XAUDIO2_WINDOWS_PROCESSOR_SPECIFIER = XAUDIO2_PROCESSOR;
}

#pragma pack(push, 1)

namespace REX::W32
{
	struct XAUDIO2_VOICE_DETAILS
	{
		std::uint32_t creationFlags;
		std::uint32_t inputChannels;
		std::uint32_t inputSampleRate;
	};

	struct XAUDIO2_SEND_DESCRIPTOR
	{
		std::uint32_t flags;
		void*         outputVoice;
	};

	struct XAUDIO2_VOICE_SENDS
	{
		std::uint32_t            sendCount;
		XAUDIO2_SEND_DESCRIPTOR* sendDescriptors;
	};

	struct XAUDIO2_FILTER_PARAMETERS
	{
		XAUDIO2_FILTER_TYPE type;
		float               frequency;
		float               oneOverQ;
	};

	struct XAUDIO2_EFFECT_DESCRIPTOR
	{
		void*         effect;
		BOOL          initialState;
		std::uint32_t outputChannels;
	};

	struct XAUDIO2_EFFECT_CHAIN
	{
		std::uint32_t              effectCount;
		XAUDIO2_EFFECT_DESCRIPTOR* effectDescriptors;
	};

	struct XAUDIO2_BUFFER
	{
		std::uint32_t       flags;
		std::uint32_t       audioBytes;
		const std::uint8_t* audioData;
		std::uint32_t       playBegin;
		std::uint32_t       playLength;
		std::uint32_t       loopBegin;
		std::uint32_t       loopLength;
		std::uint32_t       loopCount;
		void*               context;
	};

	struct XAUDIO2_BUFFER_WMA
	{
		const std::uint32_t* decodedPacketCumulativeBytes;
		std::uint32_t        packetCount;
	};

	struct XAUDIO2_VOICE_STATE
	{
		void*         currentBufferContext;
		std::uint32_t buffersQueued;
		std::uint64_t samplesPlayed;
	};

	struct XAUDIO2_PERFORMANCE_DATA
	{
		std::uint64_t audioCyclesSinceLastQuery;
		std::uint64_t totalCyclesSinceLastQuery;
		std::uint32_t minimumCyclesPerQuantum;
		std::uint32_t maximumCyclesPerQuantum;
		std::uint32_t memoryUsageInBytes;
		std::uint32_t currentLatencyInSamples;
		std::uint32_t glitchesSinceEngineStarted;
		std::uint32_t activeSourceVoiceCount;
		std::uint32_t totalSourceVoiceCount;
		std::uint32_t activeSubmixVoiceCount;
		std::uint32_t activeResamplerCount;
		std::uint32_t activeMatrixMixCount;
		std::uint32_t activeXmaSourceVoices;
		std::uint32_t activeXmaStreams;
	};

	struct XAUDIO2_DEBUG_CONFIGURATION
	{
		std::uint32_t traceMask;
		std::uint32_t breakMask;
		BOOL          logThreadID;
		BOOL          logFileline;
		BOOL          logFunctionName;
		BOOL          logTiming;
	};

	struct XAUDIO2_DEVICE_DETAILS
	{
		wchar_t              deviceID[256];
		wchar_t              displayName[256];
		XAUDIO2_DEVICE_ROLE  role;
		WAVEFORMATEXTENSIBLE outputFormat;
	};
}

#pragma pack(pop)

namespace REX::W32
{
	struct __declspec(novtable)
	IXAudio2EngineCallback
	{
		virtual void OnProcessingPassStart() noexcept = 0;
		virtual void OnProcessingPassEnd() noexcept = 0;
		virtual void OnCriticalError(HRESULT a_error) noexcept = 0;
	};

	struct __declspec(novtable, uuid("84ac29bb-d619-44d2-b197-e4acf7df3ed6"))
	IXAudio2Extension : public IUnknown
	{
		virtual void GetProcessingQuantum(std::uint32_t* a_quantumNumerator, std::uint32_t* a_quantumDenominator) noexcept = 0;
		virtual void GetProcessor(XAUDIO2_PROCESSOR* a_processor) noexcept = 0;
	};

	struct __declspec(novtable)
	IXAudio2VoiceCallback
	{
		virtual void OnVoiceProcessingPassStart(std::uint32_t a_bytesRequired) noexcept = 0;
		virtual void OnVoiceProcessingPassEnd() noexcept = 0;
		virtual void OnStreamEnd() noexcept = 0;
		virtual void OnBufferStart(void* a_bufferContext) noexcept = 0;
		virtual void OnBufferEnd(void* a_bufferContext) noexcept = 0;
		virtual void OnLoopEnd(void* a_bufferContext) noexcept = 0;
		virtual void OnVoiceError(void* a_bufferContext, HRESULT a_error) noexcept = 0;
	};

	struct __declspec(novtable, uuid("8bcf1f58-9fe7-4583-8ac6-e2adc465c8bb"))
	IXAudio27 : public IUnknown
	{
		virtual HRESULT GetDeviceCount(std::uint32_t* a_count) const = 0;
		virtual HRESULT GetDeviceDetails(std::uint32_t a_index, XAUDIO2_DEVICE_DETAILS* a_deviceDetails) const = 0;
		virtual HRESULT Initialize(std::uint32_t a_flags = 0, XAUDIO2_PROCESSOR XAudio2Processor = XAUDIO2_DEFAULT_PROCESSOR) noexcept = 0;
		virtual HRESULT RegisterForCallbacks(IXAudio2EngineCallback* pCallback) noexcept = 0;
		virtual void    UnregisterForCallbacks(IXAudio2EngineCallback* pCallback) noexcept = 0;
		virtual HRESULT CreateSourceVoice(IXAudio27SourceVoice** a_sourceVoice, const WAVEFORMATEX* a_sourceFormat, std::uint32_t a_flags = 0, float a_maxFrequencyRatio = 2.0f, IXAudio2VoiceCallback* a_callback = nullptr, const XAUDIO2_VOICE_SENDS* a_sendList = nullptr, const XAUDIO2_EFFECT_CHAIN* a_effectChain = nullptr) noexcept = 0;
		virtual HRESULT CreateSubmixVoice(IXAudio27SubmixVoice** a_submixVoice, std::uint32_t a_inputChannels, std::uint32_t a_inputSampleRate, std::uint32_t a_flags = 0, std::uint32_t a_processingStage = 0, const XAUDIO2_VOICE_SENDS* a_sendList = nullptr, const XAUDIO2_EFFECT_CHAIN* a_effectChain = nullptr) noexcept = 0;
		virtual HRESULT CreateMasteringVoice(IXAudio27MasteringVoice** a_masteringVoice, std::uint32_t a_inputChannels = XAUDIO2_DEFAULT_CHANNELS, std::uint32_t a_inputSampleRate = XAUDIO2_DEFAULT_SAMPLERATE, std::uint32_t a_flags = 0, std::uint32_t a_index = 0, const XAUDIO2_EFFECT_CHAIN* a_effectChain = nullptr) noexcept = 0;
		virtual HRESULT StartEngine() noexcept = 0;
		virtual void    StopEngine() noexcept = 0;
		virtual HRESULT CommitChanges(std::uint32_t a_operationSet) noexcept = 0;
		virtual void    GetPerformanceData(XAUDIO2_PERFORMANCE_DATA* a_perfData) const noexcept = 0;
		virtual void    SetDebugConfiguration(const XAUDIO2_DEBUG_CONFIGURATION* a_debugConfiguration, void* a_reserved = nullptr) noexcept = 0;
	};

	struct __declspec(novtable)
	IXAudio27Voice
	{
		virtual void    GetVoiceDetails(XAUDIO2_VOICE_DETAILS* a_voiceDetails) const noexcept = 0;
		virtual HRESULT SetOutputVoices(const XAUDIO2_VOICE_SENDS* a_sendList) noexcept = 0;
		virtual HRESULT SetEffectChain(const XAUDIO2_EFFECT_CHAIN* a_effectChain) noexcept = 0;
		virtual HRESULT EnableEffect(std::uint32_t a_effectIndex, std::uint32_t a_operationSet = 0) noexcept = 0;
		virtual HRESULT DisableEffect(std::uint32_t a_effectIndex, std::uint32_t a_operationSet = 0) noexcept = 0;
		virtual void    GetEffectState(std::uint32_t a_effectIndex, BOOL* a_enabled) const noexcept = 0;
		virtual HRESULT SetEffectParameters(std::uint32_t a_effectIndex, const void* a_parameters, std::uint32_t a_parametersByteSize, std::uint32_t a_operationSet = 0) noexcept = 0;
		virtual HRESULT GetEffectParameters(std::uint32_t a_effectIndex, void* a_parameters, std::uint32_t a_parametersByteSize) const noexcept = 0;
		virtual HRESULT SetFilterParameters(const XAUDIO2_FILTER_PARAMETERS* a_parameters, std::uint32_t a_operationSet = 0) noexcept = 0;
		virtual void    GetFilterParameters(XAUDIO2_FILTER_PARAMETERS* a_parameters) const noexcept = 0;
		virtual HRESULT SetOutputFilterParameters(IXAudio27Voice* a_destinationVoice, const XAUDIO2_FILTER_PARAMETERS* a_parameters, std::uint32_t a_operationSet = 0) noexcept = 0;
		virtual void    GetOutputFilterParameters(IXAudio27Voice* a_destinationVoice, XAUDIO2_FILTER_PARAMETERS* a_parameters) const noexcept = 0;
		virtual HRESULT SetVolume(float a_volume, std::uint32_t a_operationSet = 0) noexcept = 0;
		virtual void    GetVolume(float* a_volume) const noexcept = 0;
		virtual HRESULT SetChannelVolumes(std::uint32_t a_channels, const float* a_volumes, std::uint32_t a_operationSet = 0) noexcept = 0;
		virtual void    GetChannelVolumes(std::uint32_t a_channels, float* a_volumes) const noexcept = 0;
		virtual HRESULT SetOutputMatrix(IXAudio27Voice* a_destinationVoice, std::uint32_t a_sourceChannels, std::uint32_t a_destinationChannels, const float* a_levelMatrix, std::uint32_t a_operationSet = 0) noexcept = 0;
		virtual void    GetOutputMatrix(IXAudio27Voice* a_destinationVoice, std::uint32_t a_sourceChannels, std::uint32_t a_destinationChannels, float* a_levelMatrix) const noexcept = 0;
		virtual void    DestroyVoice() noexcept = 0;
	};

	struct __declspec(novtable)
	IXAudio27SourceVoice : public IXAudio27Voice
	{
		virtual HRESULT Start(std::uint32_t a_flags = 0, std::uint32_t a_operationSet = 0) noexcept = 0;
		virtual HRESULT Stop(std::uint32_t a_flags = 0, std::uint32_t a_operationSet = 0) noexcept = 0;
		virtual HRESULT SubmitSourceBuffer(XAUDIO2_BUFFER* a_buffer, const XAUDIO2_BUFFER_WMA* a_bufferWMA = nullptr) noexcept = 0;
		virtual HRESULT FlushSourceBuffers() noexcept = 0;
		virtual HRESULT Discontinuity() noexcept = 0;
		virtual HRESULT ExitLoop(std::uint32_t a_operationSet = 0) noexcept = 0;
		virtual void    GetState(XAUDIO2_VOICE_STATE* a_state) const noexcept = 0;
		virtual HRESULT SetFrequencyRatio(float a_ratio, std::uint32_t a_operationSet = 0) noexcept = 0;
		virtual void    GetFrequencyRatio(float* a_ratio) const noexcept = 0;
		virtual HRESULT SetSourceSampleRate(std::uint32_t a_rate) noexcept = 0;
	};

	struct __declspec(novtable)
	IXAudio27SubmixVoice : public IXAudio27Voice
	{};

	struct __declspec(novtable)
	IXAudio27MasteringVoice : public IXAudio27Voice
	{};
}

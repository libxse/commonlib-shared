#pragma once

#include "REX/W32/BASE.h"

namespace REX::W32
{
	struct IXAPO;
	struct IXAPOParameters;
}

namespace REX::W32
{
	inline constexpr auto XAPO_REGISTRATION_STRING_LENGTH = 256;
}

namespace REX::W32
{
	enum XAPO_BUFFER_FLAGS
	{
		XAPO_BUFFER_SILENT,
		XAPO_BUFFER_VALID,
	};
}

namespace REX::W32
{
	struct XAPO_REGISTRATION_PROPERTIES
	{
		IID           clsid;
		wchar_t       friendlyName[XAPO_REGISTRATION_STRING_LENGTH];
		wchar_t       copyrightInfo[XAPO_REGISTRATION_STRING_LENGTH];
		std::uint32_t majorVersion;
		std::uint32_t minorVersion;
		std::uint32_t flags;
		std::uint32_t minInputBufferCount;
		std::uint32_t maxInputBufferCount;
		std::uint32_t minOutputBufferCount;
		std::uint32_t maxOutputBufferCount;
	};

	struct XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS
	{
		const WAVEFORMATEX* format;
		std::uint32_t       maxFrameCount;
	};

	struct XAPO_PROCESS_BUFFER_PARAMETERS
	{
		void*             buffer;
		XAPO_BUFFER_FLAGS bufferFlags;
		std::uint32_t     validFrameCount;
	};
}

namespace REX::W32
{
	struct __declspec(novtable, uuid("a410b984-9839-4819-a0be-2856ae6b3adb"))
	IXAPO : public IUnknown
	{
		virtual HRESULT       GetRegistrationProperties(XAPO_REGISTRATION_PROPERTIES** a_registrationProperties) noexcept = 0;
		virtual HRESULT       IsInputFormatSupported(const WAVEFORMATEX* a_outputFormat, const WAVEFORMATEX* a_requestedInputFormat, WAVEFORMATEX** a_supportedInputFormat) noexcept = 0;
		virtual HRESULT       IsOutputFormatSupported(const WAVEFORMATEX* a_inputFormat, const WAVEFORMATEX* a_requestedOutputFormat, WAVEFORMATEX** a_supportedOutputFormat) noexcept = 0;
		virtual HRESULT       Initialize(const void* a_data, uint32_t a_dataByteSize) noexcept = 0;
		virtual void          Reset() noexcept = 0;
		virtual HRESULT       LockForProcess(std::uint32_t a_inputLockedParameterCount, const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* a_inputLockedParameters, std::uint32_t a_outputLockedParameterCount, const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* a_outputLockedParameters) noexcept = 0;
		virtual void          UnlockForProcess() noexcept = 0;
		virtual void          Process(std::uint32_t a_inputProcessParameterCount, const XAPO_PROCESS_BUFFER_PARAMETERS* a_InputProcessParameters, std::uint32_t a_outputProcessParameterCount, XAPO_PROCESS_BUFFER_PARAMETERS* a_outputProcessParameters, BOOL a_isEnabled) noexcept = 0;
		virtual std::uint32_t CalcInputFrames(std::uint32_t a_outputFrameCount) noexcept = 0;
		virtual std::uint32_t CalcOutputFrames(std::uint32_t a_inputFrameCount) noexcept = 0;
	};

	struct __declspec(novtable, uuid("26d95c66-80f2-499a-ad54-5ae7f01c6d98"))
	IXAPOParameters : public IUnknown
	{
		virtual void SetParameters(const void* a_params, std::uint32_t a_paramsLen) noexcept = 0;
		virtual void GetParameters(void* a_params, std::uint32_t a_paramsLen) noexcept = 0;
	};
}

#pragma once

#include <windows.h>
#include <atomic>
#include <bcrypt.h>

namespace Resonance::Proxy
{
	inline constexpr wchar_t systemLibrarySuffix[] = L"\\bcrypt.dll";
	inline std::atomic<HMODULE> systemModule{nullptr};
	inline std::atomic<FARPROC> randomFunction{nullptr};

	FARPROC ResolveRandom();

	extern "C" BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved);
	extern "C" NTSTATUS WINAPI ProxyBCryptGenRandom(
		BCRYPT_ALG_HANDLE algorithm, PUCHAR buffer, ULONG count, ULONG flags);
}

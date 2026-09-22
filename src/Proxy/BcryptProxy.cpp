#include "BcryptProxy.h"
#include <atomic>
#include <bcrypt.h>
#include <bit>
#include <cstddef>
#include <type_traits>
#if defined(_MSC_VER)
#include "Exports/Bcrypt.h"
#include <intrin.h>
#endif

#include "../Game/EndpointPatcher.h"

namespace Resonance::Proxy
{
	FARPROC ResolveRandom()
	{
		auto address = randomFunction.load(std::memory_order_acquire);

		if (address)
			return address;

		auto module = systemModule.load(std::memory_order_acquire);

		if (!module)
		{
			wchar_t path[MAX_PATH];
			const UINT length = GetSystemDirectoryW(path, MAX_PATH);

			if (!length || length + std::size(systemLibrarySuffix) > MAX_PATH)
			{
				return nullptr;
			}

			for (std::size_t i = 0; i < std::size(systemLibrarySuffix); ++i)
			{
				path[length + i] = systemLibrarySuffix[i];
			}

			module = LoadLibraryExW(path, nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);

			if (!module)
				return nullptr;

			if (HMODULE expected = nullptr; !systemModule.compare_exchange_strong(expected, module))
			{
				FreeLibrary(module);
				module = expected;
			}
		}

		address = GetProcAddress(module, "BCryptGenRandom");

		if (address)
			randomFunction.store(address, std::memory_order_release);

		return address;
	}

	extern "C" BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID)
	{
		return TRUE;
	}

	extern "C" NTSTATUS WINAPI ProxyBCryptGenRandom(
		BCRYPT_ALG_HANDLE algorithm, PUCHAR buffer, ULONG count, ULONG flags)
	{
		const DWORD lastError = GetLastError();
#if defined(_MSC_VER)
		void* caller = _ReturnAddress();
#else
		void* caller = __builtin_extract_return_addr(__builtin_return_address(0));
#endif
		Resonance::Game::EndpointPatcher::TryInitialize(caller);
		const auto address = ResolveRandom();

		if (!address)
		{
			SetLastError(lastError);

			return static_cast<NTSTATUS>(0xC0000001UL);
		}

		const auto function = std::bit_cast<decltype(&::BCryptGenRandom)>(address);
		SetLastError(lastError);

		return function(algorithm, buffer, count, flags);
	}

	static_assert(std::is_same_v<decltype(&ProxyBCryptGenRandom), decltype(&::BCryptGenRandom)>);
}

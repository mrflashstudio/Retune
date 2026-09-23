#include "ShellLink.h"
#include "../Scanning/Image.h"
#include <algorithm>
#include <bit>
#include <cstring>
#include <limits>

namespace Retune::Browser
{
	bool ShellLink::Rewrite(std::wstring_view url, std::wstring& replacement)
	{
		if (!url.starts_with(originalUrl))
			return false;

		replacement.assign(L"https://");
		replacement.append(serverDomain);
		replacement.append(url.substr(originalUrl.size()));

		return true;
	}

	BOOL WINAPI ShellLink::Open(SHELLEXECUTEINFOW* info)
	{
		const auto lastError = GetLastError();

		if (!info || info->cbSize != sizeof(SHELLEXECUTEINFOW) || !info->lpFile ||
			(info->fMask & SEE_MASK_IDLIST))
			return original(info);

		std::wstring replacement;

		if (!Rewrite(info->lpFile, replacement))
		{
			SetLastError(lastError);

			return original(info);
		}

		auto redirected = *info;
		redirected.lpFile = replacement.c_str();
		SetLastError(lastError);
		const auto result = original(&redirected);
		const auto resultError = GetLastError();
		info->hInstApp = redirected.hInstApp;
		info->hProcess = redirected.hProcess;
		SetLastError(resultError);

		return result;
	}

	std::uint8_t* ShellLink::AllocateRelay(HMODULE module)
	{
		SYSTEM_INFO system{};
		GetSystemInfo(&system);
		const auto base = reinterpret_cast<std::uintptr_t>(module);
		const auto granularity = static_cast<std::uintptr_t>(system.dwAllocationGranularity);
		const auto maximum = std::min(reinterpret_cast<std::uintptr_t>(system.lpMaximumApplicationAddress),
			base + std::numeric_limits<std::uint32_t>::max());

		for (auto address = base + granularity; address <= maximum - relaySize;)
		{
			MEMORY_BASIC_INFORMATION memory{};

			if (!VirtualQuery(reinterpret_cast<void*>(address), &memory, sizeof(memory)))
				break;

			if (memory.State == MEM_FREE)
			{
				const auto region = static_cast<std::uint8_t*>(VirtualAlloc(
					reinterpret_cast<void*>(address), relaySize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));

				if (region)
					return region;
			}

			const auto end = reinterpret_cast<std::uintptr_t>(memory.BaseAddress) + memory.RegionSize;
			const auto next = (end + granularity - 1) & ~(granularity - 1);

			if (next <= address)
				break;

			address = next;
		}

		return nullptr;
	}

	std::uint32_t* ShellLink::FindExport(HMODULE module)
	{
		Scanning::Image image;

		if (!image.Open(module))
			return nullptr;

		const auto base = reinterpret_cast<std::uint8_t*>(module);
		const auto dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(base);
		const auto pe = reinterpret_cast<const IMAGE_NT_HEADERS64*>(base + dos->e_lfanew);
		const auto directory = pe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
		const auto exports = reinterpret_cast<const IMAGE_EXPORT_DIRECTORY*>(base + directory.VirtualAddress);

		if (!image.Contains(exports, sizeof(*exports)))
			return nullptr;

		const auto names = reinterpret_cast<const std::uint32_t*>(base + exports->AddressOfNames);
		const auto ordinals = reinterpret_cast<const std::uint16_t*>(base + exports->AddressOfNameOrdinals);
		const auto functions = reinterpret_cast<std::uint32_t*>(base + exports->AddressOfFunctions);

		if (!image.Contains(names, std::size_t(exports->NumberOfNames) * sizeof(*names)) ||
			!image.Contains(ordinals, std::size_t(exports->NumberOfNames) * sizeof(*ordinals)) ||
			!image.Contains(functions, std::size_t(exports->NumberOfFunctions) * sizeof(*functions)))
			return nullptr;

		for (std::size_t i = 0; i < exports->NumberOfNames; ++i)
		{
			const auto name = reinterpret_cast<const char*>(base + names[i]);

			if (image.Contains(name, sizeof("ShellExecuteExW")) &&
				!std::memcmp(name, "ShellExecuteExW", sizeof("ShellExecuteExW")) &&
				ordinals[i] < exports->NumberOfFunctions)
				return functions + ordinals[i];
		}

		return nullptr;
	}

	bool ShellLink::Install(std::wstring_view domain)
	{
		serverDomain.assign(domain);

		const auto module = LoadLibraryExW(L"shell32.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);

		if (!module)
			return false;

		original = std::bit_cast<Execute>(GetProcAddress(module, "ShellExecuteExW"));
		const auto entry = FindExport(module);

		if (!original || !entry)
			return false;

		const auto relay = AllocateRelay(module);

		if (!relay)
			return false;

		std::memcpy(relay, relayCode.data(), relayCode.size());
		const auto target = std::bit_cast<std::uintptr_t>(&Open);
		std::memcpy(relay + relayAddressOffset, &target, sizeof(target));
		DWORD previous;

		if (!VirtualProtect(relay, relaySize, PAGE_EXECUTE_READ, &previous) ||
			!FlushInstructionCache(GetCurrentProcess(), relay, relayCode.size()) ||
			!VirtualProtect(entry, sizeof(*entry), PAGE_READWRITE, &previous))
		{
			VirtualFree(relay, 0, MEM_RELEASE);

			return false;
		}

		const auto relative =
			reinterpret_cast<std::uintptr_t>(relay) - reinterpret_cast<std::uintptr_t>(module);
		InterlockedExchange(reinterpret_cast<volatile LONG*>(entry),
			std::bit_cast<LONG>(static_cast<std::uint32_t>(relative)));
		DWORD ignored;

		return VirtualProtect(entry, sizeof(*entry), previous, &ignored) != FALSE;
	}
}

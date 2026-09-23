#pragma once

#include <windows.h>
#include <array>
#include <cstdint>
#include <shellapi.h>
#include <string>
#include <string_view>

namespace Resonance::Browser
{
	class ShellLink
	{
		using Execute = BOOL(WINAPI*)(SHELLEXECUTEINFOW*);

		static constexpr std::wstring_view originalUrl = L"https://rhythia.com";
		static constexpr std::size_t relaySize = 4096;
		static constexpr std::array<std::uint8_t, 12> relayCode = {
			0x48, 0xB8, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xE0};
		static constexpr std::size_t relayAddressOffset = 2;

		static inline Execute original = nullptr;
		static inline std::wstring serverDomain;

		static bool Rewrite(std::wstring_view url, std::wstring& replacement);
		static BOOL WINAPI Open(SHELLEXECUTEINFOW* info);
		static std::uint8_t* AllocateRelay(HMODULE module);
		static std::uint32_t* FindExport(HMODULE module);

	public:
		ShellLink() = delete;
		static bool Install(std::wstring_view domain);
	};
}

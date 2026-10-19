#pragma once

#include <cstdint>
#include <map>
#include <mutex>
#include <string>
#include <string_view>

namespace Retune::Game
{
	class StringRedirector
	{
		static constexpr std::wstring_view originalDomain = L"rhythia.com";
		static constexpr std::wstring_view signHost = L"sign.rhythia.com";
		static constexpr std::wstring_view userAgentPrefix = L"Rhythia/";

		std::wstring serverDomain;
		std::uint8_t* frozenRegistration;
		std::mutex replacementMutex;
		std::map<std::wstring, void*, std::less<>> replacements;

		static bool IsHostCharacter(wchar_t character);
		std::wstring ReplaceDomain(std::wstring_view text) const;

	public:
		StringRedirector(std::wstring_view domain, std::uint8_t* registration);
		void* Redirect(void* value);
	};
}

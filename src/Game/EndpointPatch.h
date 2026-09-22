#pragma once

#include <array>
#include <cstdint>
#include <string_view>

namespace Resonance::Game
{
	struct Endpoint
	{
		std::wstring_view original;
		wchar_t replacement[300]{};
		std::uint8_t* instruction = nullptr;
		std::int32_t displacement = 0;
	};

	struct EndpointPatch
	{
		std::uint8_t* registration = nullptr;
		void* stringType = nullptr;
		std::array<Endpoint, 5> endpoints = {{{L"https://production.rhythia.com/"},
			{L"https://production.rhythia.com/api/"}, {L"https://www.rhythia.com/"},
			{L"wss://socket.rhythia.com/"}, {L"wss://socketdev.rhythia.com/"}}};
	};
}

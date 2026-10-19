#pragma once

#include <windows.h>
#include "Failure.h"
#include "PatchTargets.h"
#include <string_view>

namespace Retune::Game
{
	class PatchDiscovery
	{
		static constexpr std::string_view registrationPattern =
			"48 83 EC ?? 48 89 4C 24 ?? 48 8B 0D ?? ?? ?? ?? 48 89 54 24 ?? 48 8D 54 24 ?? "
			"48 C7 44 24 ?? 08 00 00 00 4C 89 44 24 ?? 4C 89 4C 24 ?? 48 8B 01 "
			"48 8B 80 ?? ?? ?? ?? FF 15 ?? ?? ?? ?? 48 83 C4 ?? C3";

		static constexpr std::string_view decoderPattern =
			"41 57 41 56 57 56 55 53 48 83 EC 28 8B D9 48 8D 05 ?? ?? ?? ?? "
			"48 83 78 F8 00 0F 85 ?? ?? ?? ?? 48 8B 35 ?? ?? ?? ?? 48 8B 46 ?? "
			"3B 58 08 0F 83 ?? ?? ?? ?? 8B D3 48 8B 44 D0 10 48 85 C0";

	public:
		PatchDiscovery() = delete;
		static Failure Discover(HMODULE module, PatchTargets& targets);
	};
}

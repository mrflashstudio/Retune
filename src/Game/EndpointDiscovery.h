#pragma once

#include "../Scanning/Image.h"
#include "EndpointPatch.h"
#include "Failure.h"

namespace Retune::Game
{
	inline constexpr std::string_view registrationPattern =
		"48 83 EC ?? 48 89 4C 24 ?? 48 8B 0D ?? ?? ?? ?? 48 89 54 24 ?? 48 8D 54 24 ?? "
		"48 C7 44 24 ?? 08 00 00 00 4C 89 44 24 ?? 4C 89 4C 24 ?? 48 8B 01 "
		"48 8B 80 ?? ?? ?? ?? FF 15 ?? ?? ?? ?? 48 83 C4 ?? C3";

	inline constexpr std::wstring_view originalDomain = L"rhythia.com";

	bool DiscoverRuntime(const Scanning::Image& image, EndpointPatch& patch);
	bool DiscoverEndpoint(const Scanning::Image& image, Endpoint& endpoint, void*& stringType);
	bool ReplaceDomain(Endpoint& endpoint, std::wstring_view domain);

	Failure PrepareEndpoints(HMODULE module, std::wstring_view domain, EndpointPatch& patch);
}

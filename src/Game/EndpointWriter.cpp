#include <windows.h>
#include "EndpointWriter.h"
#include "../NativeAot/FrozenStrings.h"
#include <cstring>
#include <limits>

namespace Retune::Game
{
	Failure ApplyEndpoints(EndpointPatch& patch)
	{
		std::array<NativeAot::FrozenString, 5> strings;

		for (std::size_t i = 0; i < strings.size(); ++i)
			strings[i].value = patch.endpoints[i].replacement;

		NativeAot::FrozenStrings storage;

		if (!storage.Create(patch.endpoints.front().instruction, patch.stringType, strings))
			return Failure::Memory;

		for (std::size_t i = 0; i < strings.size(); ++i)
		{
			auto& endpoint = patch.endpoints[i];
			const auto displacement = reinterpret_cast<std::intptr_t>(strings[i].object) -
				reinterpret_cast<std::intptr_t>(endpoint.instruction + 7);

			if (displacement < std::numeric_limits<std::int32_t>::min() ||
				displacement > std::numeric_limits<std::int32_t>::max())
				return Failure::Memory;

			endpoint.displacement = static_cast<std::int32_t>(displacement);
		}

		if (!storage.Register(patch.registration))
			return Failure::Registration;

		for (const auto& endpoint : patch.endpoints)
		{
			DWORD originalProtection;

			if (!VirtualProtect(endpoint.instruction, 7, PAGE_EXECUTE_READWRITE, &originalProtection))
				return Failure::Memory;

			std::memcpy(endpoint.instruction + 3, &endpoint.displacement, sizeof(endpoint.displacement));
			DWORD ignored;

			if (!VirtualProtect(endpoint.instruction, 7, originalProtection, &ignored) ||
				!FlushInstructionCache(GetCurrentProcess(), endpoint.instruction, 7))
				return Failure::Memory;
		}

		return Failure::None;
	}
}

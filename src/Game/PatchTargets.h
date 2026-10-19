#pragma once

#include <cstdint>
#include <vector>

namespace Retune::Game
{
	struct PatchTargets
	{
		std::uint8_t* registration = nullptr;
		std::vector<std::uint8_t*> decoders;
	};
}

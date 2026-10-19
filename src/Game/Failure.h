#pragma once

#include <cstdint>

namespace Retune::Game
{
	enum class Failure : std::uint32_t
	{
		None = 0,
		Arguments = 0x1000,
		Discovery,
		Memory,
		Registration,
		ConcurrentInitialization,
		Hooks
	};

	[[noreturn]] void Fail(Failure failure);
	void RequireSuccess(Failure failure);
}

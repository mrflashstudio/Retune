#pragma once

#include <atomic>

namespace Retune::Game
{
	class Patcher
	{
		enum class State
		{
			Uninitialized,
			Initializing,
			Passthrough,
			Applied
		};

		static inline std::atomic<State> state{State::Uninitialized};

		static State Initialize();
		static bool IsGameCaller(const void* address);

	public:
		Patcher() = delete;
		static void TryInitialize(const void* returnAddress);
	};
}

#pragma once

#include "EndpointPatch.h"
#include "Failure.h"
#include <atomic>

namespace Resonance::Game
{
	class EndpointPatcher
	{
		enum class State
		{
			Uninitialized,
			Initializing,
			Passthrough,
			Applied
		};

		static inline std::atomic<State> state{State::Uninitialized};
		static inline EndpointPatch patch;

		static void RequireSuccess(Failure failure);
		static State Initialize();
		static bool IsGameCaller(const void* address);

	public:
		EndpointPatcher() = delete;
		static void TryInitialize(const void* returnAddress);
	};
}

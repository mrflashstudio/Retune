#include <windows.h>
#include "EndpointPatcher.h"
#include "../Arguments/ServerArguments.h"
#include "EndpointDiscovery.h"
#include "EndpointWriter.h"
#include <atomic>

namespace Resonance::Game
{
	void EndpointPatcher::RequireSuccess(Failure failure)
	{
		if (failure == Failure::None)
			return;

		TerminateProcess(GetCurrentProcess(), static_cast<UINT>(failure));

		for (;;)
		{
		}
	}

	EndpointPatcher::State EndpointPatcher::Initialize()
	{
		Arguments::Server server;
		const auto result = Arguments::ReadServer(server);

		if (result == Arguments::Result::Absent)
			return State::Passthrough;

		if (result != Arguments::Result::Valid)
			RequireSuccess(Failure::Arguments);

		RequireSuccess(PrepareEndpoints(GetModuleHandleW(nullptr), server.domain, patch));
		RequireSuccess(ApplyEndpoints(patch));

		return State::Applied;
	}

	bool EndpointPatcher::IsGameCaller(const void* address)
	{
		MEMORY_BASIC_INFORMATION caller{};

		return VirtualQuery(address, &caller, sizeof(caller)) &&
			caller.AllocationBase == GetModuleHandleW(nullptr);
	}

	void EndpointPatcher::TryInitialize(const void* returnAddress)
	{
		auto current = state.load(std::memory_order_acquire);

		if (current == State::Passthrough || current == State::Applied || !IsGameCaller(returnAddress))
			return;

		current = State::Uninitialized;

		if (!state.compare_exchange_strong(current, State::Initializing))
		{
			if (current == State::Passthrough || current == State::Applied)
				return;

			RequireSuccess(Failure::ConcurrentInitialization);
		}

		state.store(Initialize(), std::memory_order_release);
	}
}

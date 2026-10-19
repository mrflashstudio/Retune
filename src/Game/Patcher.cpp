#include <windows.h>
#include "Patcher.h"
#include "../Arguments/ServerArguments.h"
#include "PatchDiscovery.h"
#include "StringDecoderHooks.h"
#include <atomic>

namespace Retune::Game
{
	Patcher::State Patcher::Initialize()
	{
		Arguments::Server server;
		const auto result = Arguments::ReadServer(server);

		if (result == Arguments::Result::Absent)
			return State::Passthrough;

		if (result != Arguments::Result::Valid)
			RequireSuccess(Failure::Arguments);

		PatchTargets targets;
		RequireSuccess(PatchDiscovery::Discover(GetModuleHandleW(nullptr), targets));
		RequireSuccess(StringDecoderHooks::Install(targets, server.domain));

		return State::Applied;
	}

	bool Patcher::IsGameCaller(const void* address)
	{
		MEMORY_BASIC_INFORMATION caller{};

		return VirtualQuery(address, &caller, sizeof(caller)) &&
			caller.AllocationBase == GetModuleHandleW(nullptr);
	}

	void Patcher::TryInitialize(const void* returnAddress)
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

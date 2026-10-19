#include <windows.h>
#include "StringDecoderHooks.h"
#include "StringRedirector.h"
#include <MinHook.h>
#include <bit>
#include <cstring>
#include <limits>
#include <memory>

namespace Retune::Game
{
	void* StringDecoderHooks::DecodeString(const std::uint32_t id, const Decoder original)
	{
		const auto value = original(id);

		try
		{
			return redirector->Redirect(value);
		}
		catch (...)
		{
			Fail(Failure::Memory);
		}
	}

	Failure StringDecoderHooks::Install(const PatchTargets& targets, const std::wstring_view domain)
	{
		if (targets.decoders.empty())
			return Failure::Discovery;

		redirector = std::make_unique<StringRedirector>(domain, targets.registration);

		if (targets.decoders.size() > std::numeric_limits<std::size_t>::max() / adapterStride)
			return Failure::Memory;

		const auto size = targets.decoders.size() * adapterStride;
		const auto hooks =
			static_cast<std::uint8_t*>(VirtualAlloc(nullptr, size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE));

		if (!hooks)
			return Failure::Memory;

		for (std::size_t i = 0; i < targets.decoders.size(); ++i)
		{
			const auto hook = hooks + i * adapterStride;
			std::memcpy(hook, entryHook, sizeof(entryHook));
			const auto decode = std::bit_cast<void*>(&DecodeString);
			std::memcpy(hook + callbackOffset, &decode, sizeof(decode));
		}

		if (MH_Initialize() != MH_OK)
			return Failure::Hooks;

		for (std::size_t i = 0; i < targets.decoders.size(); ++i)
		{
			const auto hook = hooks + i * adapterStride;
			void* trampoline = nullptr;

			if (MH_CreateHook(targets.decoders[i], hook, &trampoline) != MH_OK)
				return Failure::Hooks;

			std::memcpy(hook + originalOffset, &trampoline, sizeof(trampoline));

			if (MH_QueueEnableHook(targets.decoders[i]) != MH_OK)
				return Failure::Hooks;
		}

		DWORD previous;

		if (!VirtualProtect(hooks, size, PAGE_EXECUTE_READ, &previous) ||
			!FlushInstructionCache(GetCurrentProcess(), hooks, size))
			return Failure::Memory;

		return MH_ApplyQueued() == MH_OK ? Failure::None : Failure::Hooks;
	}
}

#pragma once

#include "Failure.h"
#include "PatchTargets.h"
#include "StringRedirector.h"
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string_view>

namespace Retune::Game
{
	class StringDecoderHooks
	{
		using Decoder = void* (*)(std::uint32_t);

		static inline std::unique_ptr<StringRedirector> redirector;

		static constexpr std::size_t adapterStride = 32;
		static constexpr std::size_t originalOffset = 2;
		static constexpr std::size_t callbackOffset = 12;

		static constexpr std::uint8_t entryHook[] = {
			0x48, 0xBA, 0, 0, 0, 0, 0, 0, 0, 0, // mov rdx, original
			0x48, 0xB8, 0, 0, 0, 0, 0, 0, 0, 0, // mov rax, DecodeString
			0xFF, 0xE0							// jmp rax
		};

		static void* DecodeString(std::uint32_t id, Decoder original);

	public:
		StringDecoderHooks() = delete;
		static Failure Install(const PatchTargets& targets, std::wstring_view domain);
	};
}

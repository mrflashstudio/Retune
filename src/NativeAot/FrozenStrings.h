#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <string_view>

namespace Retune::NativeAot
{
	struct FrozenString
	{
		std::wstring_view value;
		std::uint8_t* object = nullptr;
	};

	class FrozenStrings
	{
		using RegisterFrozen = void* (*)(void*, std::size_t, std::size_t, std::size_t);

		static constexpr std::size_t regionSize = 4096;
		static constexpr std::uintptr_t allocationRange = 0x70000000;

		std::uint8_t* region = nullptr;
		std::size_t used = 0;
		bool registered = false;

		static std::uint8_t* AllocateNear(const std::uint8_t* address);

	public:
		FrozenStrings() = default;
		~FrozenStrings();
		FrozenStrings(const FrozenStrings&) = delete;
		FrozenStrings& operator=(const FrozenStrings&) = delete;
		bool Create(const std::uint8_t* nearAddress, void* stringType, std::span<FrozenString> strings);
		bool Register(std::uint8_t* registration);
	};
}

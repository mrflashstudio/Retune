#pragma once

#include <cstddef>
#include <cstdint>

namespace Resonance::NativeAot
{
	struct StringLayout
	{
		static constexpr std::size_t alignment = sizeof(void*);
		static constexpr std::size_t headerSize = sizeof(std::uintptr_t);
		static constexpr std::size_t lengthOffset = sizeof(void*);
		static constexpr std::size_t charactersOffset = lengthOffset + sizeof(std::uint32_t);
		static constexpr std::size_t allocationOverhead = headerSize + charactersOffset + sizeof(wchar_t);
		static constexpr std::size_t alignmentMask = alignment - 1;

		static constexpr std::size_t AllocationSize(std::size_t length)
		{
			return (length * sizeof(wchar_t) + allocationOverhead + alignmentMask) & ~alignmentMask;
		}
	};
}

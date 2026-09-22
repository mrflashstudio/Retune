#include <windows.h>
#include "FrozenStrings.h"
#include "StringLayout.h"
#include <bit>
#include <cstdint>
#include <cstring>

namespace Resonance::NativeAot
{
	FrozenStrings::~FrozenStrings()
	{
		if (region && !registered)
			VirtualFree(region, 0, MEM_RELEASE);
	}

	std::uint8_t* FrozenStrings::AllocateNear(const std::uint8_t* address)
	{
		SYSTEM_INFO system{};
		GetSystemInfo(&system);
		const auto granularity = static_cast<std::uintptr_t>(system.dwAllocationGranularity);
		const auto center = reinterpret_cast<std::uintptr_t>(address) & ~(granularity - 1);
		const auto minimum = reinterpret_cast<std::uintptr_t>(system.lpMinimumApplicationAddress);
		const auto maximum = reinterpret_cast<std::uintptr_t>(system.lpMaximumApplicationAddress);

		if (center < minimum || center > maximum)
			return nullptr;

		for (std::uintptr_t distance = granularity; distance < allocationRange; distance += granularity)
		{
			for (int direction = 0; direction < 2; ++direction)
			{
				if ((direction && center < distance) || (!direction && maximum - center < distance))
					continue;

				const auto candidate = direction ? center - distance : center + distance;

				if (candidate < minimum || candidate > maximum || regionSize - 1 > maximum - candidate)
					continue;

				const auto allocation =
					static_cast<std::uint8_t*>(VirtualAlloc(reinterpret_cast<void*>(candidate), regionSize,
						MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));

				if (allocation)
					return allocation;
			}
		}

		return nullptr;
	}

	bool FrozenStrings::Create(
		const std::uint8_t* nearAddress, void* stringType, std::span<FrozenString> strings)
	{
		if (region || strings.empty() || !stringType)
			return false;

		region = AllocateNear(nearAddress);

		if (!region)
			return false;

		for (auto& string : strings)
		{
			if (string.value.size() >
				(regionSize - StringLayout::allocationOverhead - StringLayout::alignmentMask) /
					sizeof(wchar_t))
				return false;

			const std::size_t stride = StringLayout::AllocationSize(string.value.size());

			if (stride > regionSize - used)
				return false;

			string.object = region + used + StringLayout::headerSize;
			std::memcpy(string.object, &stringType, sizeof(stringType));
			const auto length = static_cast<std::uint32_t>(string.value.size());
			std::memcpy(string.object + StringLayout::lengthOffset, &length, sizeof(length));
			std::memcpy(string.object + StringLayout::charactersOffset, string.value.data(),
				string.value.size() * sizeof(wchar_t));
			used += stride;
		}

		return true;
	}

	bool FrozenStrings::Register(std::uint8_t* registration)
	{
		if (!region || !used || registered || !registration)
			return false;

		const auto registerFrozen = std::bit_cast<RegisterFrozen>(registration);
		registered = registerFrozen(region, used, regionSize, regionSize) != nullptr;

		return registered;
	}
}

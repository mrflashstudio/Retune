#include <windows.h>
#include "FrozenString.h"
#include "StringLayout.h"
#include <bit>
#include <cstring>

namespace Retune::NativeAot
{
	FrozenString::~FrozenString()
	{
		if (region && !registered)
			VirtualFree(region, 0, MEM_RELEASE);
	}

	bool FrozenString::Create(void* stringType, const std::wstring_view value)
	{
		if (region || !stringType ||
			value.size() > (regionSize - StringLayout::allocationOverhead - StringLayout::alignmentMask) /
					sizeof(wchar_t))
			return false;

		region = static_cast<std::uint8_t*>(
			VirtualAlloc(nullptr, regionSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));

		if (!region)
			return false;

		const auto object = region + StringLayout::headerSize;
		const auto length = static_cast<std::uint32_t>(value.size());
		std::memcpy(object, &stringType, sizeof(stringType));
		std::memcpy(object + StringLayout::lengthOffset, &length, sizeof(length));
		std::memcpy(object + StringLayout::charactersOffset, value.data(), value.size() * sizeof(wchar_t));
		used = StringLayout::AllocationSize(value.size());

		return true;
	}

	bool FrozenString::Register(std::uint8_t* registration)
	{
		if (!region || !used || registered || !registration)
			return false;

		const auto registerFrozen = std::bit_cast<RegisterFrozen>(registration);
		registered = registerFrozen(region, used, regionSize, regionSize) != nullptr;

		return registered;
	}

	void* FrozenString::Object() const
	{
		return region ? region + StringLayout::headerSize : nullptr;
	}
}

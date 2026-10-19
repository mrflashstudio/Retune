#include <windows.h>
#include "Image.h"
#include "Pattern.h"
#include <algorithm>
#include <cstdint>
#include <cstring>

namespace Retune::Scanning
{
	bool Image::Open(const HMODULE module)
	{
		count = 0;
		const auto base = reinterpret_cast<std::uint8_t*>(module);
		MEMORY_BASIC_INFORMATION memory{};

		if (!VirtualQuery(base, &memory, sizeof(memory)) || memory.RegionSize < sizeof(IMAGE_DOS_HEADER))
			return false;

		const auto dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(base);

		if (dos->e_magic != IMAGE_DOS_SIGNATURE || dos->e_lfanew <= 0 ||
			static_cast<std::size_t>(dos->e_lfanew) + sizeof(IMAGE_NT_HEADERS64) > memory.RegionSize)
			return false;

		const auto pe = reinterpret_cast<const IMAGE_NT_HEADERS64*>(base + dos->e_lfanew);

		if (pe->Signature != IMAGE_NT_SIGNATURE || pe->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR64_MAGIC)
			return false;

		const auto sections = IMAGE_FIRST_SECTION(pe);
		const auto headersEnd =
			reinterpret_cast<const std::uint8_t*>(sections + pe->FileHeader.NumberOfSections);

		if (headersEnd > base + memory.RegionSize)
			return false;

		for (unsigned i = 0; i < pe->FileHeader.NumberOfSections; ++i)
		{
			const auto& section = sections[i];

			if (!(section.Characteristics & IMAGE_SCN_MEM_READ))
				continue;

			const std::size_t length = section.Misc.VirtualSize;

			if (section.VirtualAddress > pe->OptionalHeader.SizeOfImage ||
				length > pe->OptionalHeader.SizeOfImage - section.VirtualAddress)
				return false;

			auto cursor = base + section.VirtualAddress;
			const auto end = cursor + length;

			while (cursor < end)
			{
				if (!VirtualQuery(cursor, &memory, sizeof(memory)))
					return false;

				const auto next =
					std::min(end, static_cast<std::uint8_t*>(memory.BaseAddress) + memory.RegionSize);

				if (next <= cursor)
					return false;

				if (memory.State == MEM_COMMIT && !(memory.Protect & (PAGE_NOACCESS | PAGE_GUARD)))
				{
					if (count == std::size(ranges))
						return false;

					ranges[count++] = {cursor, static_cast<std::size_t>(next - cursor),
						(section.Characteristics & IMAGE_SCN_MEM_EXECUTE) != 0};
				}

				cursor = next;
			}
		}

		return count != 0;
	}

	std::uint8_t* Image::FindUnique(const std::string_view pattern, const bool executable) const
	{
		std::uint8_t* result = nullptr;
		const auto parsedPattern = Pattern::Parse(pattern);

		if (!parsedPattern)
			return nullptr;

		for (std::size_t i = 0; i < count; ++i)
		{
			const auto& range = ranges[i];

			if (range.executable != executable)
				continue;

			auto remaining = std::span<const std::uint8_t>(range.address, range.size);

			while (const auto match = parsedPattern->FindFirst(remaining))
			{
				if (result)
					return nullptr;

				result = const_cast<std::uint8_t*>(match);
				remaining = remaining.subspan(static_cast<std::size_t>(match - remaining.data()) + 1);
			}
		}

		return result;
	}

	std::vector<std::uint8_t*> Image::FindAll(const std::string_view pattern, const bool executable) const
	{
		std::vector<std::uint8_t*> result;
		const auto parsed = Pattern::Parse(pattern);

		if (!parsed)
			return result;

		for (std::size_t i = 0; i < count; ++i)
		{
			if (ranges[i].executable != executable)
				continue;

			auto remaining = std::span<const std::uint8_t>(ranges[i].address, ranges[i].size);

			while (const auto match = parsed->FindFirst(remaining))
			{
				result.push_back(const_cast<std::uint8_t*>(match));
				remaining = remaining.subspan(static_cast<std::size_t>(match - remaining.data()) + 1);
			}
		}

		return result;
	}
}

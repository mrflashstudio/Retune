#pragma once

#include <windows.h>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string_view>

namespace Retune::Scanning
{
	struct Range
	{
		std::uint8_t* address;
		std::size_t size;
		bool executable;
	};

	class Image
	{
		static constexpr std::size_t rangeCapacity = 256;

		Range ranges[rangeCapacity]{};
		std::size_t count = 0;

	public:
		bool Open(HMODULE module);
		bool Contains(const void* address, std::size_t size, bool executable = false) const;
		std::uint8_t* FindUnique(std::string_view pattern, bool executable) const;
		std::uint8_t* FindString(std::wstring_view value) const;
		std::uint8_t* FindReference(const void* target) const;
	};
}

#pragma once

#include <windows.h>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string_view>
#include <vector>

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
		std::uint8_t* FindUnique(std::string_view pattern, bool executable) const;
		std::vector<std::uint8_t*> FindAll(std::string_view pattern, bool executable) const;
	};
}

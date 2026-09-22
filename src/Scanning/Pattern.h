#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <span>
#include <string_view>

namespace Resonance::Scanning
{
	class Pattern
	{
		static constexpr std::size_t capacity = 256;
		static constexpr std::string_view whitespace = " \t\r\n\v\f";

		std::array<std::uint8_t, capacity> bytes{};
		std::array<bool, capacity> wildcards{};
		std::size_t length = 0;
		std::size_t anchor = 0;
		std::size_t anchorLength = 0;

		bool Matches(const std::uint8_t* address) const;
		static int HexDigit(char character);

	public:
		static std::optional<Pattern> Parse(std::string_view text);
		const std::uint8_t* FindFirst(std::span<const std::uint8_t> memory) const;
	};
}

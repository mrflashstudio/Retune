#include "Pattern.h"
#include <cstring>

namespace Retune::Scanning
{
	int Pattern::HexDigit(char character)
	{
		if (character >= '0' && character <= '9')
			return character - '0';

		if (character >= 'a' && character <= 'f')
			return character - 'a' + 10;

		if (character >= 'A' && character <= 'F')
			return character - 'A' + 10;

		return -1;
	}

	std::optional<Pattern> Pattern::Parse(std::string_view text)
	{
		Pattern pattern;
		std::size_t runLength = 0;

		while (!text.empty())
		{
			const auto start = text.find_first_not_of(whitespace);

			if (start == std::string_view::npos)
				break;

			text.remove_prefix(start);
			const auto end = text.find_first_of(whitespace);
			const auto token = text.substr(0, end);

			if (pattern.length == pattern.bytes.size())
				return std::nullopt;

			const bool wildcard = token == "?" || token == "??";

			if (!wildcard)
			{
				if (token.size() != 2)
					return std::nullopt;

				const int high = HexDigit(token[0]);
				const int low = HexDigit(token[1]);

				if (high < 0 || low < 0)
					return std::nullopt;

				pattern.bytes[pattern.length] = static_cast<std::uint8_t>((high << 4) | low);
			}

			pattern.wildcards[pattern.length++] = wildcard;
			runLength = wildcard ? 0 : runLength + 1;

			if (runLength > pattern.anchorLength)
			{
				pattern.anchor = pattern.length - runLength;
				pattern.anchorLength = runLength;
			}

			if (end == std::string_view::npos)
				break;

			text.remove_prefix(end);
		}

		if (!pattern.length)
			return std::nullopt;

		return pattern;
	}

	bool Pattern::Matches(const std::uint8_t* address) const
	{
		for (std::size_t i = 0; i < length; ++i)
		{
			if (!wildcards[i] && address[i] != bytes[i])
				return false;
		}

		return true;
	}

	const std::uint8_t* Pattern::FindFirst(const std::span<const std::uint8_t> memory) const
	{
		if (!length || memory.size() < length)
			return nullptr;

		if (!anchorLength)
			return memory.data();

		auto cursor = memory.data() + anchor;
		const auto last = memory.data() + memory.size() - length + anchor;

		while (cursor <= last)
		{
			cursor = static_cast<const std::uint8_t*>(
				std::memchr(cursor, bytes[anchor], static_cast<std::size_t>(last - cursor) + 1));

			if (!cursor)
				return nullptr;

			if (std::memcmp(cursor, bytes.data() + anchor, anchorLength) == 0 && Matches(cursor - anchor))
				return cursor - anchor;

			++cursor;
		}

		return nullptr;
	}
}

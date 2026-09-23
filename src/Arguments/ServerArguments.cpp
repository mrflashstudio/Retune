#include <windows.h>
#include "ServerArguments.h"
#include <shellapi.h>

namespace Retune::Arguments
{
	bool IsDomain(std::wstring_view value)
	{
		if (value.empty() || value.size() > maxDomainLength)
			return false;

		std::size_t labelLength = 0;
		bool hasLetter = false;
		wchar_t previous = 0;

		for (const auto character : value)
		{
			const bool letter =
				(character >= L'a' && character <= L'z') || (character >= L'A' && character <= L'Z');

			const bool digit = character >= L'0' && character <= L'9';
			hasLetter |= letter;

			if (character == L'.')
			{
				if (!labelLength || previous == L'-')
					return false;

				labelLength = 0;
			}
			else
			{
				if (!letter && !digit && character != L'-')
					return false;

				if ((!labelLength && character == L'-') || ++labelLength > maxLabelLength)
					return false;
			}

			previous = character;
		}

		return labelLength && previous != L'-' && hasLetter;
	}

	Result ReadServer(Server& server)
	{
		int count = 0;
		const auto arguments = CommandLineToArgvW(GetCommandLineW(), &count);

		if (!arguments)
			return Result::Invalid;

		auto result = Result::Absent;

		for (int i = 1; i < count; ++i)
		{
			const std::wstring_view argument = arguments[i];
			std::wstring_view value;

			if (argument == serverFlag)
			{
				if (++i == count)
				{
					result = Result::Invalid;
					break;
				}

				value = arguments[i];
			}
			else if (argument.starts_with(assignedServerFlag))
				value = argument.substr(serverFlag.size() + 1);
			else
				continue;

			if (result != Result::Absent || !IsDomain(value))
			{
				result = Result::Invalid;
				break;
			}

			for (std::size_t j = 0; j < value.size(); ++j)
			{
				const auto character = value[j];
				server.domain[j] =
					character >= L'A' && character <= L'Z' ? character + (L'a' - L'A') : character;
			}

			server.domain[value.size()] = 0;
			result = Result::Valid;
		}

		LocalFree(arguments);

		return result;
	}
}

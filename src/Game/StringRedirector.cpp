#include "StringRedirector.h"
#include "../NativeAot/FrozenString.h"
#include "../NativeAot/StringLayout.h"
#include "Failure.h"
#include <cstring>

namespace Retune::Game
{
	StringRedirector::StringRedirector(const std::wstring_view domain, std::uint8_t* registration)
		: serverDomain(domain), frozenRegistration(registration)
	{
	}

	bool StringRedirector::IsHostCharacter(const wchar_t character)
	{
		return (character >= L'a' && character <= L'z') || (character >= L'A' && character <= L'Z') ||
			(character >= L'0' && character <= L'9') || character == L'-' || character == L'.';
	}

	std::wstring StringRedirector::ReplaceDomain(const std::wstring_view text) const
	{
		std::wstring replacement;
		std::size_t copied = 0;
		std::size_t position = 0;

		while ((position = text.find(originalDomain, position)) != std::wstring_view::npos)
		{
			const auto end = position + originalDomain.size();
			auto hostStart = position;

			while (hostStart && IsHostCharacter(text[hostStart - 1]))
				--hostStart;

			const bool boundary =
				(!position || text[position - 1] == L'.' || !IsHostCharacter(text[position - 1])) &&
				(end == text.size() || !IsHostCharacter(text[end]));

			if (boundary && text.substr(hostStart, end - hostStart) != signHost)
			{
				replacement.append(text.substr(copied, position - copied));
				replacement.append(serverDomain);
				copied = end;
			}

			position = end;
		}

		if (copied)
			replacement.append(text.substr(copied));

		return replacement;
	}

	void* StringRedirector::Redirect(void* value)
	{
		if (!value)
			return value;

		const auto object = static_cast<const std::uint8_t*>(value);
		std::uint32_t length;
		std::memcpy(&length, object + NativeAot::StringLayout::lengthOffset, sizeof(length));

		const std::wstring_view text(
			reinterpret_cast<const wchar_t*>(object + NativeAot::StringLayout::charactersOffset), length);

		if (text.starts_with(userAgentPrefix) || text.find(originalDomain) == std::wstring_view::npos)
			return value;

		const std::lock_guard lock(replacementMutex);
		const auto existing = replacements.find(text);

		if (existing != replacements.end())
			return existing->second;

		const auto replacement = ReplaceDomain(text);

		if (replacement.empty())
			return value;

		void* type;
		std::memcpy(&type, object, sizeof(type));

		const auto entry = replacements.emplace(text, nullptr).first;
		NativeAot::FrozenString string;

		if (!string.Create(type, replacement))
			Fail(Failure::Memory);

		if (!string.Register(frozenRegistration))
			Fail(Failure::Registration);

		entry->second = string.Object();

		return string.Object();
	}
}

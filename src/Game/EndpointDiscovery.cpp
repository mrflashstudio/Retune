#include "EndpointDiscovery.h"
#include "../Arguments/ServerArguments.h"
#include "../NativeAot/StringLayout.h"
#include "../Scanning/Image.h"
#include <cstring>

namespace Resonance::Game
{
	bool DiscoverRuntime(const Scanning::Image& image, EndpointPatch& patch)
	{
		patch.registration = image.FindUnique(registrationPattern, true);

		return patch.registration != nullptr;
	}

	bool DiscoverEndpoint(const Scanning::Image& image, Endpoint& endpoint, void*& stringType)
	{
		const auto characters = image.FindString(endpoint.original);

		if (!characters)
			return false;

		const auto object = reinterpret_cast<std::uint8_t*>(
			reinterpret_cast<std::uintptr_t>(characters) - NativeAot::StringLayout::charactersOffset);
		const auto header = reinterpret_cast<std::uint8_t*>(
			reinterpret_cast<std::uintptr_t>(object) - NativeAot::StringLayout::headerSize);

		if (!image.Contains(header,
				NativeAot::StringLayout::headerSize + NativeAot::StringLayout::charactersOffset +
					(endpoint.original.size() + 1) * sizeof(wchar_t)))
			return false;

		std::uintptr_t syncBlock;
		std::uint32_t length;
		void* type;
		std::memcpy(&syncBlock, header, sizeof(syncBlock));
		std::memcpy(&type, object, sizeof(type));
		std::memcpy(&length, object + NativeAot::StringLayout::lengthOffset, sizeof(length));

		if (syncBlock || length != endpoint.original.size() || !image.Contains(type, sizeof(void*)) ||
			(stringType && type != stringType))
			return false;

		stringType = type;
		endpoint.instruction = image.FindReference(object);

		return endpoint.instruction != nullptr;
	}

	bool ReplaceDomain(Endpoint& endpoint, const std::wstring_view domain)
	{
		const auto domainOffset = endpoint.original.find(originalDomain);

		if (domainOffset == std::wstring_view::npos)
			return false;

		const auto suffix = endpoint.original.substr(domainOffset + originalDomain.size());
		const auto newLength = domainOffset + domain.size() + suffix.size();

		if (newLength >= std::size(endpoint.replacement))
			return false;

		std::memcpy(endpoint.replacement, endpoint.original.data(), domainOffset * sizeof(wchar_t));
		std::memcpy(endpoint.replacement + domainOffset, domain.data(), domain.size() * sizeof(wchar_t));
		std::memcpy(endpoint.replacement + domainOffset + domain.size(), suffix.data(),
			suffix.size() * sizeof(wchar_t));

		endpoint.replacement[newLength] = 0;
		const std::wstring_view replacement = endpoint.replacement;
		const auto hostStart = replacement.find(L"://") + 3;
		const auto hostEnd = replacement.find(L'/', hostStart);

		return Arguments::IsDomain(replacement.substr(hostStart, hostEnd - hostStart));
	}

	Failure PrepareEndpoints(const HMODULE module, const std::wstring_view domain, EndpointPatch& patch)
	{
		patch = EndpointPatch{};
		Scanning::Image image;

		if (!image.Open(module) || !DiscoverRuntime(image, patch))
			return Failure::Discovery;

		for (auto& endpoint : patch.endpoints)
		{
			if (!DiscoverEndpoint(image, endpoint, patch.stringType))
				return Failure::Discovery;

			if (!ReplaceDomain(endpoint, domain))
				return Failure::Arguments;
		}

		return Failure::None;
	}
}

#pragma once

#include <string_view>

namespace Retune::Arguments
{
	inline constexpr std::size_t maxDomainLength = 253;
	inline constexpr std::size_t maxLabelLength = 63;
	inline constexpr std::wstring_view serverFlag = L"--resonance-server";
	inline constexpr std::wstring_view assignedServerFlag = L"--resonance-server=";

	enum class Result
	{
		Absent,
		Valid,
		Invalid
	};

	struct Server
	{
		wchar_t domain[maxDomainLength + 1]{};
	};

	bool IsDomain(std::wstring_view value);
	Result ReadServer(Server& server);
}

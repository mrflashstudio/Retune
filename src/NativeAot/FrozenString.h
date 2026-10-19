#pragma once

#include <cstddef>
#include <cstdint>
#include <string_view>

namespace Retune::NativeAot
{
	class FrozenString
	{
		using RegisterFrozen = void* (*)(void*, std::size_t, std::size_t, std::size_t);

		static constexpr std::size_t regionSize = 4096;

		std::uint8_t* region = nullptr;
		std::size_t used = 0;
		bool registered = false;

	public:
		FrozenString() = default;
		~FrozenString();
		FrozenString(const FrozenString&) = delete;
		FrozenString& operator=(const FrozenString&) = delete;
		bool Create(void* stringType, std::wstring_view value);
		bool Register(std::uint8_t* registration);
		void* Object() const;
	};
}

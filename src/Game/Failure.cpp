#include <windows.h>
#include "Failure.h"
#include <exception>

namespace Retune::Game
{
	[[noreturn]] void Fail(const Failure failure)
	{
		TerminateProcess(GetCurrentProcess(), static_cast<UINT>(failure));
		std::terminate();
	}

	void RequireSuccess(const Failure failure)
	{
		if (failure != Failure::None)
			Fail(failure);
	}
}

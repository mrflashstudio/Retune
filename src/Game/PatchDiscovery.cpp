#include "PatchDiscovery.h"
#include "../Scanning/Image.h"

namespace Retune::Game
{
	Failure PatchDiscovery::Discover(const HMODULE module, PatchTargets& targets)
	{
		targets = PatchTargets{};
		Scanning::Image image;

		if (!image.Open(module))
			return Failure::Discovery;

		targets.registration = image.FindUnique(registrationPattern, true);
		targets.decoders = image.FindAll(decoderPattern, true);

		if (!targets.registration || targets.decoders.empty())
			return Failure::Discovery;

		return Failure::None;
	}
}

#include "ServiceLocator.h"
#include "NullSoundSystem.h"

namespace dae
{
	std::unique_ptr<SoundSystem> ServiceLocator::_ss_instance = std::make_unique<NullSoundSystem>();
}
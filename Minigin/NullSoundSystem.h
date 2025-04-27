#pragma once
#include "SoundSystem.h"
namespace dae
{
	class NullSoundSystem : public SoundSystem
	{
		void Play(const SoundId, const float) override {};
		void AddAudioClip(SoundId, const std::string&) override {};
	};
}


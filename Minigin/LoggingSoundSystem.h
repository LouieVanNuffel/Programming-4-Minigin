#pragma once
#include "SoundSystem.h"
#include <memory>
#include <iostream>
namespace dae
{
	class LoggingSoundSystem : public SoundSystem
	{
		std::unique_ptr<SoundSystem> _real_ss;
	public:
		LoggingSoundSystem(std::unique_ptr<SoundSystem>&& ss)
			: _real_ss(std::move(ss)) {}
		virtual ~LoggingSoundSystem() = default;

		void AddAudioClip(SoundId id, const std::string& filepath) override
		{
			_real_ss->AddAudioClip(id, filepath);
			std::cout << "added audio clip " << id << " from path " << filepath << std::endl;
		}

		void Play(const SoundId id, const float volume) override
		{
			_real_ss->Play(id, volume);
			std::cout << "playing " << id << " at volume " << volume << std::endl;
		}
	};
}


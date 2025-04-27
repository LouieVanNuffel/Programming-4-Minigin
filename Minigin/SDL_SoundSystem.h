#pragma once
#include "SoundSystem.h"
#include <unordered_map>
#include <memory>
#include "AudioClip.h"
#include <thread>
namespace dae
{
	class SDL_SoundSystem : public SoundSystem
	{
	public:
		void AddAudioClip(SoundId id, const std::string& filepath) override
		{
			m_AudioClips[id] = std::make_unique<AudioClip>(filepath);
		}

		void Play(const SoundId id, const float volume) override
		{
			auto it = m_AudioClips.find(id);
			if (it == m_AudioClips.end()) return;

			auto& audioclip = it->second;
			if (!audioclip->IsLoaded()) audioclip->Load();

			audioclip->SetVolume(volume);
			audioclip->Play();
		}
		
	private:
		std::unordered_map<SoundId, std::unique_ptr<AudioClip>> m_AudioClips;
	};
}


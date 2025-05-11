#pragma once
#include "QueuedSoundSystemImplementation.cpp"
namespace dae
{
	class QueuedSoundSystem : public SoundSystem
	{
	public:
		QueuedSoundSystem()
		{
			m_pQueuedSoundSystemImpl = std::make_unique<QueuedSoundSystemImplementation>();
		}

		void AddAudioClip(SoundId id, const std::string& filepath) override
		{
			m_pQueuedSoundSystemImpl->AddAudioClip(id, filepath);
		}

		void Play(const SoundId id, const float volume) override
		{
			m_pQueuedSoundSystemImpl->Play(id, volume);
		}

	private:
		std::unique_ptr<QueuedSoundSystemImplementation> m_pQueuedSoundSystemImpl;

	};
}


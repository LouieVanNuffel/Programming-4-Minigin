#pragma once
#include "SoundSystem.h"
#include <unordered_map>
#include <memory>
#include "AudioClip.h"
#include <queue>
#include <thread>
#include <mutex>
namespace dae
{
	struct AudioPlayRequest
	{
		SoundId id;
		float volume;
	};

	class QueuedSoundSystem : public SoundSystem
	{
	public:
		QueuedSoundSystem()
		{
			m_AudioThread = std::thread{ &QueuedSoundSystem::CycleQueue, this };
		}

		~QueuedSoundSystem()
		{
			if (m_AudioThread.joinable())
				m_AudioThread.join();
		}

		void AddAudioClip(SoundId id, const std::string& filepath) override
		{
			m_AudioClips[id] = std::make_unique<AudioClip>(filepath);
		}

		void Play(const SoundId id, const float volume) override
		{
			std::lock_guard<std::mutex> lock(m_QueueMutex);
			m_PlayQueue.push(AudioPlayRequest{ id, volume });
		}

		void CycleQueue()
		{
			while (true)
			{
				if (!m_PlayQueue.empty())
				{
					std::queue<AudioPlayRequest> localQueue;
					{
						std::lock_guard<std::mutex> lock(m_QueueMutex);
						std::swap(localQueue, m_PlayQueue); // Steal all commands at once
					}

					while (!localQueue.empty())
					{
						const AudioPlayRequest& apr = localQueue.front();

						//play sound
						auto it = m_AudioClips.find(apr.id);
						if (it == m_AudioClips.end()) return;

						auto& audioclip = it->second;
						if (!audioclip->IsLoaded()) audioclip->Load();

						audioclip->SetVolume(apr.volume);
						audioclip->Play();

						localQueue.pop();
					}
				}
			}
		}

		private:
			std::unordered_map<SoundId, std::unique_ptr<AudioClip>> m_AudioClips;
			std::queue<AudioPlayRequest> m_PlayQueue;
			std::mutex m_QueueMutex;
			std::thread m_AudioThread;
		};
	}


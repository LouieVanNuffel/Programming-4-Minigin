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

	class QueuedSoundSystemImplementation : public SoundSystem
	{
	public:
		QueuedSoundSystemImplementation()
		{
			m_AudioThread = std::jthread{ &QueuedSoundSystemImplementation::CycleQueue, this };
			m_StopCycle.store(false);
		}

		~QueuedSoundSystemImplementation()
		{
			m_StopCycle.store(true);
			if (m_AudioThread.joinable())
				m_AudioThread.join();
		}

		void AddAudioClip(SoundId id, const std::string& filepath) override
		{
			m_AudioClips[id] = std::make_unique<AudioClip>(filepath);
		}

		void Play(const SoundId id, const float volume) override
		{
			std::unique_lock<std::mutex> lock(m_QueueMutex);
			m_PlayQueue.push(AudioPlayRequest{ id, volume });
			lock.unlock();
		}

	private:
		void CycleQueue()
		{
			while (!m_StopCycle.load())
			{
				if (!m_PlayQueue.empty())
				{
					std::queue<AudioPlayRequest> localQueue;
					{
						std::unique_lock<std::mutex> lock(m_QueueMutex);
						std::swap(localQueue, m_PlayQueue); // Steal all commands at once
						lock.unlock();
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

		std::unordered_map<SoundId, std::unique_ptr<AudioClip>> m_AudioClips;
		std::queue<AudioPlayRequest> m_PlayQueue;
		std::mutex m_QueueMutex;
		std::jthread m_AudioThread;
		std::atomic<bool> m_StopCycle;
	};
}


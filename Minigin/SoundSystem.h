#pragma once
#include <string>
namespace dae
{
	using SoundId = unsigned short;
	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;
		virtual void AddAudioClip(SoundId id, const std::string& filepath) = 0;
		virtual void Play(const SoundId id, const float volume) = 0;
	};
}


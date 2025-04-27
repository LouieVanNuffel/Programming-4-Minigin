#pragma once
#include "Command.h"
#include "ServiceLocator.h"
#include "SoundSystem.h"
namespace dae
{
	class PlaySoundCommand : public Command
	{
	public:
		PlaySoundCommand(SoundId id, float volume)
			:m_Id{ id }, m_Volume{ volume }, m_SoundSystem{ ServiceLocator::get_sound_system() } {}

		virtual void Execute() override
		{
			m_SoundSystem.Play(m_Id, m_Volume);
		}

	private:
		SoundSystem& m_SoundSystem;
		SoundId m_Id;
		float m_Volume;
	};
}
#pragma once
#include "Command.h"
#include "ServiceLocator.h"
#include "SoundSystem.h"

class PlaySoundCommand : public dae::Command
{
public:
	PlaySoundCommand(dae::SoundId id, float volume)
		:m_Id{ id }, m_Volume{ volume }, m_SoundSystem{ dae::ServiceLocator::get_sound_system() } {}

	virtual void Execute() override
	{
		m_SoundSystem.Play(m_Id, m_Volume);
	}

private:
	dae::SoundSystem& m_SoundSystem;
	dae::SoundId m_Id;
	float m_Volume;
};
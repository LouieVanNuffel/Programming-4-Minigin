#include "GameState.h"
#include "EngineTime.h"

GameState::GameState()
{
}

void GameState::Update()
{
	if (!m_CanSwitch)
	{
		m_SwitchCooldownTimer -= dae::Time::GetInstance().GetDeltaTime();
		if (m_SwitchCooldownTimer <= 0.0f) m_CanSwitch = true;
	}
}

void GameState::AddScore(int amount)
{
	m_Score += amount;
}

int GameState::Score() const
{
	return m_Score;
}

void GameState::SwitchPlayerCount()
{
	if (!m_CanSwitch) return;
	m_CanSwitch = false;
	m_SwitchCooldownTimer = m_SwitchCooldown;

	if (m_PlayerCount == 1) m_PlayerCount = 2;
	else m_PlayerCount = 1;
}

int GameState::PlayerCount() const
{
	return m_PlayerCount;
}

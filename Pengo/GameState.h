#pragma once
#include "Singleton.h"

class GameState final : public dae::Singleton<GameState>
{
public:
	GameState();

	void Update();

	void AddScore(int amount);
	int Score() const;
	void SwitchPlayerCount();
	int PlayerCount() const;

private:
	int m_Score{};
	int m_PlayerCount{ 1 };
	bool m_CanSwitch{ true };
	float m_SwitchCooldown{ 0.2f };
	float m_SwitchCooldownTimer{};
};


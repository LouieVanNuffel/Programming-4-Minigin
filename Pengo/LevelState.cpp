#include "LevelState.h"
#include "BlockComponent.h"
#include "GameObject.h"
#include "PointComponent.h"
#include <algorithm>
#include <iostream>
#include <cassert>
#include <random>

LevelState::LevelState()
{
}

void LevelState::AddPlayerObject(dae::GameObject* gameObject)
{
	m_PlayerObjects.emplace_back(gameObject);
	PointComponent* pointComponent = gameObject->GetComponent<PointComponent>();
	if (pointComponent == nullptr) return;
	m_PointComponents.emplace_back(pointComponent);
}

const std::vector<dae::GameObject*>& LevelState::GetPlayerObjects() const
{
	return m_PlayerObjects;
}

void LevelState::AddSnoBee()
{
	++m_SnoBeeCount;
}

void LevelState::RemoveSnoBee(bool hasHatched)
{
	--m_SnoBeeCount;
	std::cout << m_SnoBeeCount << std::endl;
	std::cout << std::boolalpha << hasHatched << std::endl;

	if (hasHatched == false)
	{
		AddScore(m_PointsForCrushedEgg);
		return;
	}
	else AddScore(m_PointsForSnoBeeCrushed);

	if (!HatchOneRemainingEgg() && m_SnoBeeCount <= 0)
	{
		AwardBonusPoints();
		// Do something to win the level
		std::cout << "Level completed" << std::endl;
	}
}

void LevelState::RegisterEggBlock(BlockComponent* blockComponent)
{
	m_BlockComponents.emplace_back(blockComponent);
}

void LevelState::UnregisterEggBlock(BlockComponent* blockComponent)
{
	auto it = std::find(m_BlockComponents.begin(), m_BlockComponents.end(), blockComponent);
	if (it != m_BlockComponents.end()) m_BlockComponents.erase(it);
}

void LevelState::HatchHalfOfEggs()
{
	// Randomize order
	std::random_device rd{};
	std::shuffle(m_BlockComponents.begin(), m_BlockComponents.end(), rd);

	size_t halfCount = m_BlockComponents.size() / 2;

	// Hatch eggs
	for (uint32_t index{}; index < halfCount; ++index)
	{
		m_BlockComponents[index]->Hatch();
	}
}

bool LevelState::HatchOneRemainingEgg()
{
	if (m_BlockComponents.empty()) return false;
	else
	{
		m_BlockComponents[0]->Hatch();
		return true;
	}
}

void LevelState::AddScore(int amount)
{
	for (PointComponent* playerPointComponent : m_PointComponents)
	{
		playerPointComponent->PickUpPoints(amount);
	}

	m_Score += amount;
}

int LevelState::Score() const
{
	return m_Score;
}

void LevelState::AddTime(float amount)
{
	m_Timer += amount;
}

void LevelState::AwardBonusPoints()
{
	// Round time to closest 10fold under value
	int amountOfTimeRounded = static_cast<int>(m_Timer) / 10;
	amountOfTimeRounded *= 10;

	int bonusPoints = m_TimeBonusPoints[amountOfTimeRounded];
	AddScore(bonusPoints);
}

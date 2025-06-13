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

void LevelState::RegisterTiles(const std::vector<TileInfo>& tileInfoVector)
{
	m_TilesInfo.clear();
	m_TilesInfo = tileInfoVector;
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

glm::vec3 LevelState::GetPlayerSpawnPosition() const
{
	// Return middle tile position
	size_t index = m_TilesInfo.size() / 2;
	float positionX = m_TilesInfo[index].x;
	float positionY = m_TilesInfo[index].y;
	return glm::vec3{ positionX, positionY, 0.0f };
}

glm::vec3 LevelState::GetClosestTilePositionInDirection(const glm::vec3 currentPosition, float directionX, float directionY) const
{
	if (m_TilesInfo.empty()) return glm::vec3{};

	glm::vec3 currentTilePosition = GetClosestTilePositionToPosition(currentPosition);
	glm::vec3 tilePositionInDirection = glm::vec3{ currentTilePosition.x + directionX * 24.f, 
												   currentTilePosition.y + directionY * 24.f, 0.0f };
	return tilePositionInDirection;
}

glm::vec3 LevelState::GetClosestTilePositionToPosition(const glm::vec3 position) const
{
	if (m_TilesInfo.empty()) return glm::vec3{};

	auto it = std::min_element(m_TilesInfo.begin(), m_TilesInfo.end(), [position](const TileInfo& a, const TileInfo& b) {
		return DistanceSquared(position, glm::vec3{ a.x, a.y, 0.0f }) < DistanceSquared(position, glm::vec3{ b.x, b.y, 0.0f });
		});
	return glm::vec3{ it->x, it->y, 0.0f };
}

void LevelState::AwardBonusPoints()
{
	// Round time to closest 10fold under value
	int amountOfTimeRounded = static_cast<int>(m_Timer) / 10;
	amountOfTimeRounded *= 10;

	int bonusPoints = m_TimeBonusPoints[amountOfTimeRounded];
	AddScore(bonusPoints);
}

float LevelState::DistanceSquared(const glm::vec3& posA, const glm::vec3& posB)
{
	float distanceX = posB.x - posA.x;
	float distanceY = posB.y - posA.y;
	return (powf(distanceX, 2.0f) + powf(distanceY, 2.0f));
}

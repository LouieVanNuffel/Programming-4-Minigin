#pragma once
#include "Singleton.h"
#include "Level.h"
#include <vector>
#include <map>
#include <glm.hpp>

namespace dae
{
	class GameObject;
}

class PointComponent;
class BlockComponent;
class LevelState final : public dae::Singleton<LevelState>
{
public:
	//Constructor
	LevelState();

	//Destructor
	~LevelState() = default;

	//Rule of 5
	LevelState(const LevelState& other) = delete;
	LevelState(LevelState&& other) = delete;
	LevelState& operator=(const LevelState& other) = delete;
	LevelState& operator=(LevelState&& other) = delete;

	void AddPlayerObject(dae::GameObject* gameObject);
	const std::vector<dae::GameObject*>& GetPlayerObjects() const;

	void RegisterTiles(const std::vector<TileInfo>& tileInfoVector);

	void AddSnoBee();
	void RemoveSnoBee(bool hasHatched);

	void RegisterEggBlock(BlockComponent* blockComponent);
	void UnregisterEggBlock(BlockComponent* blockComponent);

	void HatchHalfOfEggs();
	bool HatchOneRemainingEgg(); // returns false if all eggs have been hatched

	void AddScore(int amount);
	int Score() const;

	void AddTime(float amount);

	glm::vec3 GetPlayerSpawnPosition() const;
	glm::vec3 GetClosestTilePositionInDirection(const glm::vec3 currentPosition, float directionX, float directionY) const;
	glm::vec3 GetClosestTilePositionToPosition(const glm::vec3 position) const;

private:
	void AwardBonusPoints();
	static float DistanceSquared(const glm::vec3& posA, const glm::vec3& posB);

	float m_Timer{};
	int m_Score{};
	uint32_t m_SnoBeeCount{};
	std::vector<BlockComponent*> m_BlockComponents{};
	std::vector<dae::GameObject*> m_PlayerObjects{};
	std::vector<PointComponent*> m_PointComponents{};
	std::vector<TileInfo> m_TilesInfo{};

	// Points to award for snobees
	int m_PointsForSnoBeeCrushed{ 400 };
	int m_PointsForStunnedSnoBeeCrushed{ 100 };
	int m_PointsForCrushedEgg{ 500 };

	// Points to award for diamond blocks
	int m_PointsForAlignedAlongWall{ 5000 };
	int m_PointsForAlignedNOTAlongWall{ 10000 };

	// Bonus points
	// <int(time / 10) * 10, points>
	std::map<int, int> m_TimeBonusPoints{ {0, 5000}, {10, 5000}, {20, 2000}, {30, 1000}, {40, 500}, {50, 10} };
};


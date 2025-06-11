#pragma once
#include "Singleton.h"
#include <vector>
#include <map>

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
	LevelState(const LevelState& other) = default;
	LevelState(LevelState&& other) = default;
	LevelState& operator=(const LevelState& other) = default;
	LevelState& operator=(LevelState&& other) = default;

	void AddPlayerObject(dae::GameObject* gameObject);

	void AddSnoBee();
	void RemoveSnoBee(bool hasHatched);

	void RegisterEggBlock(BlockComponent* blockComponent);
	void UnregisterEggBlock(BlockComponent* blockComponent);

	void HatchHalfOfEggs();
	bool HatchOneRemainingEgg(); // returns false if all eggs have been hatched

	void AddScore(int amount);
	int Score() const;

	void AddTime(float amount);

private:
	void AwardBonusPoints();

	float m_Timer{};
	int m_Score{};
	uint32_t m_SnoBeeCount{};
	std::vector<BlockComponent*> m_BlockComponents{};
	std::vector<dae::GameObject*> m_PlayerObjects{};
	std::vector<PointComponent*> m_PointComponents{};

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


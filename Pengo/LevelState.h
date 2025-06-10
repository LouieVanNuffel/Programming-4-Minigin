#pragma once
#include "Singleton.h"
#include <vector>

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

	void AddSnoBee();
	void RemoveSnoBee(bool hasHatched);

	void RegisterEggBlock(BlockComponent* blockComponent);
	void UnregisterEggBlock(BlockComponent* blockComponent);

	void HatchHalfOfEggs();
	bool HatchOneRemainingEgg(); // returns false if all eggs have been hatched

private:
	void RemoveBlockAtIndex(uint32_t index);

	int m_Score{};
	uint32_t m_SnoBeeCount{};
	std::vector<BlockComponent*> m_BlockComponents{};
};


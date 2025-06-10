#include "LevelState.h"
#include "BlockComponent.h"
#include <algorithm>
#include <iostream>
#include <cassert>
#include <random>

LevelState::LevelState()
{
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

	if (hasHatched == false) return;

	if (!HatchOneRemainingEgg() && m_SnoBeeCount <= 0)
	{
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
	assert(it != m_BlockComponents.end());
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

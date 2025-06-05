#include "Level.h"
#include "fstream"
#include <iostream>

Level::Level(const std::string& filePath)
{
	LoadInBlockTypesPerTile("../Data/" + filePath);
	SaveLevelToFile("../Data/Levels/anothaone.pengo");
}

void Level::LoadInBlockTypesPerTile(const std::string& filePath)
{
	m_BlockTypesPerTile.reserve(m_NumberOfRows * m_NumberOfColumns);

	std::ifstream inputFile(filePath);
	int value{};
	
	if (!inputFile) return;

	while (inputFile >> value)
	{
		m_BlockTypesPerTile.emplace_back(static_cast<BlockType>(value));
	}

	inputFile.close();

	std::cout << "Level data loaded: ";
	for (const BlockType& blockType : m_BlockTypesPerTile)
	{
		std::cout << static_cast<int>(blockType) << " ";
	}
	std::cout << std::endl;
}

void Level::SaveLevelToFile(const std::string& filePath)
{
	std::ofstream outputFile(filePath);

	if (!outputFile) return;

	for (const BlockType& blockType : m_BlockTypesPerTile)
	{
		outputFile << static_cast<int>(blockType) << " ";
	}

	outputFile.close();
}

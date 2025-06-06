#include "Level.h"
#include "fstream"
#include <iostream>

Level::Level(const std::string& filePath)
{
	LoadInBlockTypesPerTileBinary("../Data/" + filePath);
}

void Level::LoadInBlockTypesPerTileText(const std::string& filePath)
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

void Level::LoadInBlockTypesPerTileBinary(const std::string& filePath)
{
	m_BlockTypesPerTile.clear();
	m_BlockTypesPerTile.reserve(m_NumberOfRows * m_NumberOfColumns);

	std::ifstream inputFile(filePath, std::ios::binary);
	
	if (!inputFile)
	{
		std::cerr << "Error opening file for reading." << std::endl;
		return;
	}

	// get file size
	inputFile.seekg(0, std::ios::end);
	std::streamsize fileSize = inputFile.tellg();
	inputFile.seekg(0, std::ios::beg);
	
	size_t numberOfBlocks = fileSize / sizeof(BlockType);

	for (int index{}; index < numberOfBlocks; ++index)
	{
		BlockType raw{};
		inputFile.read(reinterpret_cast<char*>(&raw), sizeof(BlockType));
		m_BlockTypesPerTile.emplace_back(raw);
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
	std::ofstream outputFile(filePath, std::ios::binary);

	if (!outputFile)
	{
		std::cerr << "Error opening file for writing." << std::endl;
		return;
	}

	for (int index{}; index < m_BlockTypesPerTile.size(); ++index)
	{
		outputFile.write(reinterpret_cast<const char*>(&m_BlockTypesPerTile[index]), sizeof(BlockType));
	}

	outputFile.close();
}

#include "Level.h"
#include "fstream"
#include "GameObject.h"
#include "RenderComponent.h"
#include <iostream>

Level::Level(const std::string& filePath, int blockSize, float scale, int offsetX, int offsetY)
	:m_BlockSize{ static_cast<int>(blockSize * scale) }, m_Scale{ scale }, m_OffsetX{ offsetX }, m_OffsetY{ offsetY }
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
	
	size_t numberOfBlocks = static_cast<size_t>(fileSize / sizeof(BlockType));

	for (uint32_t index{}; index < numberOfBlocks; ++index)
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

	for (uint32_t index{}; index < m_BlockTypesPerTile.size(); ++index)
	{
		outputFile.write(reinterpret_cast<const char*>(&m_BlockTypesPerTile[index]), sizeof(BlockType));
	}

	outputFile.close();
}

const std::vector<std::shared_ptr<dae::GameObject>>& Level::LoadLevelGameObjects()
{
	m_LevelGameObjects.clear();
	m_LevelGameObjects.reserve(m_BlockTypesPerTile.size());

	// Add background
	auto backgroundObject = std::make_shared<dae::GameObject>();
	auto textureComponent = std::make_unique<dae::RenderComponent>(backgroundObject.get());
	textureComponent->SetTexture("levels.png", SDL_Rect{ 0, 0, 224, 256 }, m_Scale);
	backgroundObject->AddComponent(std::move(textureComponent));
	backgroundObject->SetPosition(static_cast<float>(m_OffsetX), static_cast<float>(m_OffsetY));
	m_LevelGameObjects.emplace_back(backgroundObject);

	// Add Blocks
	for (uint32_t index{}; index < m_BlockTypesPerTile.size(); ++index)
	{
		auto blockObject = std::make_shared<dae::GameObject>();
		// The half block offset is because the edge of the background is half a block
		blockObject->SetPosition(static_cast<float>(m_OffsetX + m_BlockSize * 0.5f + GetColumnIndexFromVectorIndex(index) * m_BlockSize),
								static_cast<float>(m_OffsetY + m_BlockSize * 0.5f + GetRowIndexFromVectorIndex(index) * m_BlockSize));
		textureComponent = std::make_unique<dae::RenderComponent>(blockObject.get());
		textureComponent->SetTexture("blocks.png", SDL_Rect{ 0, 0, 16, 16 }, m_Scale);
		blockObject->AddComponent(std::move(textureComponent));
		m_LevelGameObjects.emplace_back(blockObject);
	}

	return m_LevelGameObjects;
}

int Level::GetRowIndexFromVectorIndex(int index) const
{
	return index / m_NumberOfColumns;
}

int Level::GetColumnIndexFromVectorIndex(int index) const
{
	return index - (GetRowIndexFromVectorIndex(index) * m_NumberOfColumns);
}

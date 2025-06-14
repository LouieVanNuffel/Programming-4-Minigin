#include "Level.h"
#include "fstream"
#include "GameObject.h"
#include "RenderComponent.h"
#include "BoxColliderComponent.h"
#include "BlockObject.h"
#include "LevelState.h"
#include "WallComponent.h"
#include "Layers.h"
#include "SceneManager.h"
#include "Scene.h"
#include <iostream>

Level::Level(const std::string& levelName, int blockSize, float scale, int offsetX, int offsetY)
	:m_BlockSize{ static_cast<int>(blockSize * scale) }, m_Scale{ scale }, m_OffsetX{ offsetX }, m_OffsetY{ offsetY }
{
#if _DEBUG
	LoadInBlockTypesPerTileText("../Data/Levels/" + levelName + ".txt");
	SaveLevelToFile("../Data/Levels/" + levelName + ".pengo");
#else
	LoadInBlockTypesPerTileBinary("../Data/Levels/" + levelName + ".pengo");
#endif

	InitializeTilesInfo();
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

	std::cout << "Level data loaded from " + filePath + ": ";
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

	std::cout << "Level data loaded from " + filePath + ": ";
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

	std::cout << "Level data saved to " + filePath << std::endl;
}

void Level::InitializeTilesInfo()
{
	for (uint32_t index{}; index < m_BlockTypesPerTile.size(); ++index)
	{
		// Position adds half a block size as extra offset for the background border
		float positionX = static_cast<float>(m_OffsetX + m_BlockSize * 0.5f + GetColumnIndexFromVectorIndex(index) * m_BlockSize);
		float positionY = static_cast<float>(m_OffsetY + m_BlockSize * 0.5f + GetRowIndexFromVectorIndex(index) * m_BlockSize);

		m_TilesInfo.emplace_back(TileInfo{ positionX, positionY,
											  positionX + m_BlockSize * 0.5f, positionX + m_BlockSize * 0.5f,
												m_BlockTypesPerTile[index] });
	}

	dae::SceneManager::GetInstance().ActiveScene().levelState->RegisterTiles(m_TilesInfo);
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

	// Add border colliders
	AddBorderColliders();

	// Add Blocks
	for (const TileInfo& tile : m_TilesInfo)
	{
		if (tile.blockType == BlockType::none) continue;

		auto blockObject = BlockObject{ tile.blockType, m_BlockSize, tile.x, tile.y, m_Scale }.GetGameObject();
		m_LevelGameObjects.emplace_back(blockObject);
	}

	// Hatch half of the eggs
	dae::SceneManager::GetInstance().ActiveScene().levelState->HatchHalfOfEggs();

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

void Level::AddBorderColliders()
{
	std::vector<std::pair<float, float>> positions =
	{
		{m_OffsetX + m_BlockSize * 0.5f, m_OffsetY + m_BlockSize * 0.5f - 50.0f * m_Scale}, // top left horizontal
		{m_OffsetX + m_BlockSize * 0.5f, m_OffsetY + 256 * m_Scale - m_BlockSize * 0.5f}, // bottom left
		{m_OffsetX + 224 * m_Scale - m_BlockSize * 0.5f, m_OffsetY + m_BlockSize * 0.5f}, // top right
		{m_OffsetX + m_BlockSize * 0.5f - 50.0f * m_Scale, m_OffsetY + m_BlockSize * 0.5f} // top left vertical
	};

	std::pair<float, float> horizontalBorder = std::make_pair(224.0f * m_Scale, 50.0f * m_Scale);
	std::pair<float, float> verticalBorder = std::make_pair(50.0f * m_Scale, 256.0f * m_Scale);

	float width{};
	float height{};

	for (uint32_t index{}; index < positions.size(); ++index)
	{
		auto object = std::make_shared<dae::GameObject>();
		object->SetPosition(positions[index].first, positions[index].second);

		if (index < 2) width = horizontalBorder.first, height = horizontalBorder.second;
		else width = verticalBorder.first, height = verticalBorder.second;

		auto boxColliderComponent = std::make_unique<dae::BoxColliderComponent>(width, height, dae::ObjectType::immovable, 
			object.get(), static_cast<uint32_t>(Layer::wall));
		auto wallComponent = std::make_unique<WallComponent>(object.get());

		object->AddComponent(std::move(boxColliderComponent));
		object->AddComponent(std::move(wallComponent));
		m_LevelGameObjects.emplace_back(object);
	}
}

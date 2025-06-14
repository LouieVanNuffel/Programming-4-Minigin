#pragma once
#include <string>
#include <vector>
#include <memory>
#include "BlockObject.h"

namespace dae
{
	class GameObject;
}

struct TileInfo
{
	float x;
	float y;
	float centerX;
	float centerY;
	BlockType blockType;
};

class Level final
{
public:
	//Constructor
	Level(const std::string& levelName, int blockSize, float scale = 1.0f, int offsetX = 0, int offsetY = 0);

	//Destructor
	~Level() = default;

	//Rule of 5
	Level(const Level& other) = default;
	Level(Level&& other) = default;
	Level& operator=(const Level& other) = default;
	Level& operator=(Level&& other) = default;

	const std::vector<std::shared_ptr<dae::GameObject>>& LoadLevelGameObjects();

private:
	void LoadInBlockTypesPerTileText(const std::string& filePath);
	void LoadInBlockTypesPerTileBinary(const std::string& filePath);
	void SaveLevelToFile(const std::string& filePath);

	void InitializeTilesInfo();

	int GetRowIndexFromVectorIndex(int index) const;
	int GetColumnIndexFromVectorIndex(int index) const;

	void AddBorderColliders();

	int m_NumberOfRows{ 15 };
	int m_NumberOfColumns{ 13 };
	int m_BlockSize;
	float m_Scale;
	int m_OffsetX;
	int m_OffsetY;
	std::vector<BlockType> m_BlockTypesPerTile{};
	std::vector<std::shared_ptr<dae::GameObject>> m_LevelGameObjects{};
	std::vector<TileInfo> m_TilesInfo{};

};
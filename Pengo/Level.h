#pragma once
#include <string>
#include <vector>

class Level final
{
public:
	//Constructor
	Level(const std::string& filePath);

	//Destructor
	~Level() = default;

	//Rule of 5
	Level(const Level& other) = default;
	Level(Level&& other) = default;
	Level& operator=(const Level& other) = default;
	Level& operator=(Level&& other) = default;

private:
	enum class BlockType
	{
		none, ice, egg, diamond
	};

	void LoadInBlockTypesPerTileText(const std::string& filePath);
	void LoadInBlockTypesPerTileBinary(const std::string& filePath);
	void SaveLevelToFile(const std::string& filePath);

	int m_NumberOfRows{ 15 };
	int m_NumberOfColumns{ 13 };
	std::vector<BlockType> m_BlockTypesPerTile{};

};
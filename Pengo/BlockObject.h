#pragma once
#include <memory>
#include "BlockComponent.h"

namespace dae
{
	class GameObject;
}

class BlockObject
{
public:
	//Constructor
	BlockObject(BlockType blockType, int blockSize, float positionX, float positionY, float scale);

	//Destructor
	~BlockObject() = default;

	//Rule of 5
	BlockObject(const BlockObject& other) = default;
	BlockObject(BlockObject&& other) = default;
	BlockObject& operator=(const BlockObject& other) = default;
	BlockObject& operator=(BlockObject&& other) = default;

	std::shared_ptr<dae::GameObject> GetGameObject();

private:
	std::shared_ptr<dae::GameObject> m_GameObject{};
	BlockType m_BlockType;

};


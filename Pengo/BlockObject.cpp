#include "BlockObject.h"
#include "GameObject.h"
#include "BoxColliderComponent.h"
#include "RenderComponent.h"
#include "BlockComponent.h"
#include "VelocityComponent.h"

BlockObject::BlockObject(BlockType blockType, int blockSize, float positionX, float positionY, float scale)
	:m_BlockType{ blockType }
{
	m_GameObject = std::make_shared<dae::GameObject>();
	m_GameObject->SetPosition(positionX, positionY);

	SDL_Rect sourceRect{};

	switch (m_BlockType)
	{
	case BlockType::ice:
	case BlockType::egg:
		sourceRect = SDL_Rect{ 0, 0, 32, 32 };
		break;
	case BlockType::diamond:
		sourceRect = SDL_Rect{ 32 * 2, 32 * 1, 32, 32 };
		break;
	default:
		break;
	}

	auto textureComponent = std::make_unique<dae::RenderComponent>(m_GameObject.get());
	// half the scale here because for some reason the size of the block texture is double the size in comparison with the other textures
	textureComponent->SetTexture("blocks.png", sourceRect, scale * 0.5f);

	auto boxColliderComponent = std::make_unique<dae::BoxColliderComponent>(static_cast<float>(blockSize), static_cast<float>(blockSize),
		dae::ObjectType::immovable, m_GameObject.get(), dae::Layer::block);

	auto blockComponent = std::make_unique<BlockComponent>(m_BlockType, blockSize, m_GameObject.get());
	auto velocityComponent = std::make_unique<dae::VelocityComponent>(m_GameObject.get());

	m_GameObject->AddComponent(std::move(boxColliderComponent));
	m_GameObject->AddComponent(std::move(textureComponent));
	m_GameObject->AddComponent(std::move(blockComponent));
	m_GameObject->AddComponent(std::move(velocityComponent));
}

std::shared_ptr<dae::GameObject> BlockObject::GetGameObject()
{
	return m_GameObject;
}

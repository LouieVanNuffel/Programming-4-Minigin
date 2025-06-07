#pragma once
#include "Component.h"

namespace dae
{
	class BoxColliderComponent;
}

enum class BlockType
{
	none, ice, egg, diamond
};

class BlockComponent final : public dae::Component
{
public:
	//Constructor
	BlockComponent(BlockType blockType, int blockSize, dae::GameObject* gameObject);

	//Destructor
	virtual ~BlockComponent() override = default;

	//Rule of 5
	BlockComponent(const BlockComponent& other) = delete;
	BlockComponent(BlockComponent&& other) = delete;
	BlockComponent& operator=(const BlockComponent& other) = delete;
	BlockComponent& operator=(BlockComponent&& other) = delete;

	virtual void Start() override;
	virtual void Update() override;
	virtual void LateUpdate() override {};
	virtual void Render() const override {};
	virtual void RenderUI() const override {};

	void Push(int directionX, int directionY);

private:
	void PerformRaycast();
	void SetPositionNextToBlock(const dae::BoxColliderComponent* other);
	void SetPositionNextToWall(float x, float y);

	dae::BoxColliderComponent* m_pBoxColliderComponent{ nullptr };

	BlockType m_BlockType;

	float m_Speed{ 200.0f };
	int m_BlockSize;
	int m_DirectionX{};
	int m_DirectionY{};
	bool m_IsMoving{ false };

};


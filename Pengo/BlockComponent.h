#pragma once
#include "Component.h"
#include "AnimationSequence.h"

namespace dae
{
	class BoxColliderComponent;
	class RenderComponent;
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
	void Break();
	void Hatch();

private:
	bool PerformRaycast();
	void SetPositionNextToBlock(const dae::BoxColliderComponent* other);
	void SetPositionNextToWall(float x, float y);
	void UpdateBreakAnimation();

	dae::BoxColliderComponent* m_pBoxColliderComponent{ nullptr };
	
	dae::RenderComponent* m_pRenderComponent{ nullptr };
	AnimationSequence m_BreakAnimationSequence{ 32, 0, 8, 8, false };

	BlockType m_BlockType;

	float m_Speed{ 200.0f };
	int m_BlockSize;
	int m_DirectionX{};
	int m_DirectionY{};
	bool m_IsMoving{ false };
	bool m_IsBroken{ false };
	bool m_IsHatched{ false };

};


#pragma once
#include "Component.h"

enum class BlockType
{
	none, ice, egg, diamond
};

class BlockComponent final : public dae::Component
{
public:
	//Constructor
	BlockComponent(BlockType blockType, dae::GameObject* gameObject);

	//Destructor
	virtual ~BlockComponent() override = default;

	//Rule of 5
	BlockComponent(const BlockComponent& other) = delete;
	BlockComponent(BlockComponent&& other) = delete;
	BlockComponent& operator=(const BlockComponent& other) = delete;
	BlockComponent& operator=(BlockComponent&& other) = delete;

	virtual void Start() override {};
	virtual void Update() override;
	virtual void LateUpdate() override {};
	virtual void Render() const override {};
	virtual void RenderUI() const override {};
	virtual void OnCollisionEnter(const dae::BoxColliderComponent& other);

	void Push(int directionX, int directionY);

private:
	BlockType m_BlockType;

	float m_Speed{ 200.0f };
	int m_DirectionX{};
	int m_DirectionY{};
	bool m_IsMoving{ false };

};


#pragma once
#include "GameObjectCommand.h"
#include "HealthComponent.h"
#include "TileMovementComponent.h"

class TileBasedMoveCommand final : public dae::GameObjectCommand
{
public:
	TileBasedMoveCommand(dae::GameObject* gameObject, float directionX, float directionY)
		:GameObjectCommand(gameObject), m_DirectionX{ directionX }, m_DirectionY{ directionY }
	{
		m_pHealthComponent = gameObject->GetComponent<dae::HealthComponent>();
		m_pTileMovementComponent = gameObject->GetComponent<TileMovementComponent>();
	}

	virtual void Execute() override
	{
		if (m_pTileMovementComponent == nullptr) return;

		if (m_pHealthComponent != nullptr)
		{
			if (m_pHealthComponent->GetLives() <= 0) return;
		}

		m_pTileMovementComponent->SetDirection(m_DirectionX, m_DirectionY);
	}

private:
	dae::HealthComponent* m_pHealthComponent{ nullptr };
	TileMovementComponent* m_pTileMovementComponent{ nullptr };
	float m_DirectionX;
	float m_DirectionY;
};


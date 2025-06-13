#pragma once
#include "GameObjectCommand.h"
#include "EngineTime.h"
#include "HealthComponent.h"

class MoveCommand : public dae::GameObjectCommand
{
public:
	MoveCommand(dae::GameObject* gameObject, float directionX, float directionY, float speed)
		:GameObjectCommand(gameObject), m_DirectionX{ directionX }, m_DirectionY{ directionY }, m_Speed{ speed }
	{
		m_pHealthComponent = gameObject->GetComponent<dae::HealthComponent>();
	}

	virtual void Execute() override
	{
		if (m_pHealthComponent != nullptr)
		{
			if (m_pHealthComponent->GetLives() <= 0) return;
		}

		glm::vec3 currentPosition{ GetCharacterObject()->GetTransform().GetPosition() };
		GetCharacterObject()->SetPosition(currentPosition.x + m_DirectionX * m_Speed * dae::Time::GetInstance().GetDeltaTime(),
			currentPosition.y + m_DirectionY * m_Speed * dae::Time::GetInstance().GetDeltaTime());
	}

private:
	dae::HealthComponent* m_pHealthComponent{ nullptr };
	float m_DirectionX;
	float m_DirectionY;
	float m_Speed;
};


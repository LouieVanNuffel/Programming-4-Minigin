#pragma once
#include "GameObjectCommand.h"
#include "EngineTime.h"
#include "HealthComponent.h"
#include "Event.h"

namespace dae
{
	class MoveCommand : public GameObjectCommand
	{
	public:
		MoveCommand(GameObject* gameObject, float directionX, float directionY, float speed)
			:GameObjectCommand(gameObject), m_DirectionX{ directionX }, m_DirectionY{ directionY }, m_Speed{ speed }
		{
			m_pHealthComponent = gameObject->GetComponent<HealthComponent>();
		}

		virtual void Execute() override
		{
			if (m_pHealthComponent != nullptr)
			{
				if (m_pHealthComponent->GetLives() <= 0) return;
			}

			glm::vec3 currentPosition{ GetCharacterObject()->GetTransform().GetPosition() };
			GetCharacterObject()->SetPosition(currentPosition.x + m_DirectionX * m_Speed * Time::GetInstance().GetDeltaTime(),
				currentPosition.y + m_DirectionY * m_Speed * Time::GetInstance().GetDeltaTime());
		}

	private:
		HealthComponent* m_pHealthComponent{ nullptr };
		float m_DirectionX;
		float m_DirectionY;
		float m_Speed;
	};
}


#pragma once
#include "GameObjectCommand.h"
#include "MoveComponent.h"

namespace dae
{
	class MoveCommand : public GameObjectCommand
	{
	public:
		MoveCommand(GameObject* gameObject, float directionX, float directionY)
			:GameObjectCommand(gameObject), m_DirectionX{ directionX }, m_DirectionY{ directionY }
		{
			m_MoveComponent = gameObject->GetComponent<MoveComponent>();
		}

		virtual void Execute() override
		{
			if (m_MoveComponent == nullptr) return;

			m_MoveComponent->Move(m_DirectionX, m_DirectionY);
		}

	private:
		MoveComponent* m_MoveComponent;
		float m_DirectionX;
		float m_DirectionY;
	};
}


#pragma once
#include "GameObjectCommand.h"
#include "EngineTime.h"

namespace dae
{
	class MoveCommand : public GameObjectCommand
	{
	public:
		MoveCommand(GameObject* gameObject, float directionX, float directionY, float speed)
			:GameObjectCommand(gameObject), m_DirectionX{ directionX }, m_DirectionY{ directionY }, m_Speed{ speed }
		{
		}

		virtual void Execute() override
		{
			glm::vec3 currentPosition{ GetGameObject()->GetTransform().GetPosition()};
			GetGameObject()->SetPosition(currentPosition.x + m_DirectionX * m_Speed * Time::GetInstance().GetDeltaTime(),
				currentPosition.y + m_DirectionY * m_Speed * Time::GetInstance().GetDeltaTime());
		}

	private:
		float m_DirectionX;
		float m_DirectionY;
		float m_Speed;
	};
}


#pragma once
#include "GameObjectCommand.h"
#include "EngineTime.h"
#include "Subject.h"
#include "Event.h"

namespace dae
{
	class MoveCommand : public GameObjectCommand
	{
	public:
		MoveCommand(GameObject* gameObject, float directionX, float directionY, float speed)
			:GameObjectCommand(gameObject), m_DirectionX{ directionX }, m_DirectionY{ directionY }, m_Speed{ speed }, 
			m_pSubject{ GetGameObject()->GetComponent<Subject>() }
		{
		}

		virtual void Execute() override
		{
			glm::vec3 currentPosition{ GetGameObject()->GetTransform().GetPosition()};
			GetGameObject()->SetPosition(currentPosition.x + m_DirectionX * m_Speed * Time::GetInstance().GetDeltaTime(),
				currentPosition.y + m_DirectionY * m_Speed * Time::GetInstance().GetDeltaTime());

			if (m_pSubject != nullptr)
			{
				m_pSubject->NotifyObservers(Event{ make_sdbm_hash("PlayerMoved") });

				if (m_DirectionX < 0) m_pSubject->NotifyObservers(Event{ make_sdbm_hash("PlayerLeft") });
				else if (m_DirectionX > 0) m_pSubject->NotifyObservers(Event{ make_sdbm_hash("PlayerRight") });
				else if (m_DirectionY > 0) m_pSubject->NotifyObservers(Event{ make_sdbm_hash("PlayerDown") });
				else if (m_DirectionY < 0) m_pSubject->NotifyObservers(Event{ make_sdbm_hash("PlayerUp") });
			}
		}

	private:
		Subject* m_pSubject;

		float m_DirectionX;
		float m_DirectionY;
		float m_Speed;
	};
}


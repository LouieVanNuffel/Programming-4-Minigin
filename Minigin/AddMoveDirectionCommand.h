#pragma once
#include "GameObjectCommand.h"
#include "MoveComponent.h"

namespace dae
{
	class AddMoveDirectionCommand : public GameObjectCommand
	{
	public:
		AddMoveDirectionCommand(GameObject* gameObject, float up, float down, float left, float right)
			:GameObjectCommand(gameObject), m_Up{ up }, m_Down{ down }, m_Left{ left }, m_Right{ right }
		{
			m_MoveComponent = gameObject->GetComponent<MoveComponent>();
		}

		virtual void Execute() override
		{
			if (m_MoveComponent == nullptr) return;

			if (m_Up < 0) m_MoveComponent->SetUpPressed(false);
			else if (m_Up > 0) m_MoveComponent->SetUpPressed(true);

			if (m_Down < 0) m_MoveComponent->SetDownPressed(false);
			else if (m_Down > 0) m_MoveComponent->SetDownPressed(true);

			if (m_Left < 0) m_MoveComponent->SetLeftPressed(false);
			else if (m_Left > 0) m_MoveComponent->SetLeftPressed(true);

			if (m_Right < 0) m_MoveComponent->SetRightPressed(false);
			else if (m_Right > 0) m_MoveComponent->SetRightPressed(true);
		}

	private:
		MoveComponent* m_MoveComponent;
		float m_Up;
		float m_Down;
		float m_Left;
		float m_Right;
	};
}


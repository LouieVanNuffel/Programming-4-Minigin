#pragma once
#include "GameObject.h"
#include "Command.h"

namespace dae
{
	class GameObjectCommand : public Command
	{
		GameObject* m_GameObject;
	protected:
		GameObject* GetGameObject() const { return m_GameObject; }
	public:
		GameObjectCommand(GameObject* gameObject)
		:m_GameObject{ gameObject } {}

		virtual ~GameObjectCommand() {};
	};
}


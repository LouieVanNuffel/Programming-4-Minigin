#pragma once
#include "GameObjectCommand.h"
#include "HealthComponent.h"

namespace dae
{
	class TakeDamageCommand : public GameObjectCommand
	{
	public:
		TakeDamageCommand(GameObject* gameObject, float damage)
			:GameObjectCommand(gameObject), m_Damage{ damage }
		{
			m_HealthComponent = gameObject->GetComponent<HealthComponent>();
		}

		virtual void Execute() override
		{
			if (m_HealthComponent != nullptr)
			{
				m_HealthComponent->TakeDamage(m_Damage);
			}
		}

	private:
		HealthComponent* m_HealthComponent;
		float m_Damage;
	};
}


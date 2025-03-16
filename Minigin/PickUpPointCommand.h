#pragma once
#include "GameObjectCommand.h"
#include "PointComponent.h"

namespace dae
{
	class PickUpPointCommand : public GameObjectCommand
	{
	public:
		PickUpPointCommand(GameObject* gameObject, int amountOfPoints)
			:GameObjectCommand(gameObject), m_AmountOfPoints{ amountOfPoints }
		{
			m_PointComponent = gameObject->GetComponent<PointComponent>();
		}

		virtual void Execute() override
		{
			if (m_PointComponent != nullptr)
			{
				m_PointComponent->PickUpPoints(m_AmountOfPoints);
			}
		}

	private:
		PointComponent* m_PointComponent;
		int m_AmountOfPoints;
	};
}


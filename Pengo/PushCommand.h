#pragma once
#include "GameObjectCommand.h"
#include "Event.h"
#include "CollisionSystem.h"
#include "BoxColliderComponent.h"
#include "BlockComponent.h"
#include "Animator.h"
#include <iostream>

namespace dae
{
	class PushCommand : public GameObjectCommand
	{
	public:
		PushCommand(GameObject* gameObject)
			:GameObjectCommand(gameObject)
		{
			m_pAnimator = gameObject->GetComponent<Animator>();
			assert(m_pAnimator != nullptr);
		}

		virtual void Execute() override
		{
			if (m_pAnimator == nullptr) return;

			Raycast raycast{};
			InitializeRaycast(raycast);

			std::cout << raycast.directionX << ", " << raycast.directionY << std::endl;

			HitInfo hitInfo_Out{};
			if (!CollisionSystem::GetInstance().PerformRaycast(raycast, hitInfo_Out)) return;
			if (hitInfo_Out.other == nullptr) return;

			BlockComponent* blockComponent = hitInfo_Out.other->GetGameObject()->GetComponent<BlockComponent>();
			if (blockComponent == nullptr) return;

			blockComponent->Push(raycast.directionX, raycast.directionY);
		}

	private:
		Animator* m_pAnimator{ nullptr };
		float m_Distance{ 16.0f };

		void InitializeRaycast(Raycast& raycast)
		{
			glm::vec3 currentPosition = GetCharacterObject()->GetWorldPosition();

			raycast.startX = currentPosition.x;
			raycast.startY = currentPosition.y;
			raycast.distance = m_Distance;

			switch (m_pAnimator->GetDirection())
			{
			case Direction::up:
				raycast.directionX = 0;
				raycast.directionY = -1;
				break;
			case Direction::down:
				raycast.directionX = 0;
				raycast.directionY = 1;
				break;
			case Direction::left:
				raycast.directionX = -1;
				raycast.directionY = 0;
				break;
			case Direction::right:
				raycast.directionX = 1;
				raycast.directionY = 0;
				break;
			}
		}

	};
}


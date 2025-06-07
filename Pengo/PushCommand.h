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
			m_pBoxColliderComponent = gameObject->GetComponent<BoxColliderComponent>();
			assert(m_pAnimator != nullptr && m_pBoxColliderComponent != nullptr);
		}

		virtual void Execute() override
		{
			if (m_pAnimator == nullptr || m_pBoxColliderComponent == nullptr) return;

			Raycast raycast{};
			InitializeRaycast(raycast);

			HitInfo hitInfo_Out{};
			if (!CollisionSystem::GetInstance().PerformRaycast(raycast, hitInfo_Out, m_pBoxColliderComponent)) return;
			if (hitInfo_Out.other == nullptr) return;

			BlockComponent* blockComponent = hitInfo_Out.other->GetGameObject()->GetComponent<BlockComponent>();
			if (blockComponent == nullptr) return;

			blockComponent->Push(raycast.directionX, raycast.directionY);
		}

	private:
		Animator* m_pAnimator{ nullptr };
		BoxColliderComponent* m_pBoxColliderComponent{ nullptr };
		float m_Distance{ 16.0f };

		void InitializeRaycast(Raycast& raycast)
		{
			BoxCollider boxCollider = m_pBoxColliderComponent->BoxDimensions();

			raycast.startX = boxCollider.centerX;
			raycast.startY = boxCollider.centerY;
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


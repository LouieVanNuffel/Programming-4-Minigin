#pragma once
#include "GameObjectCommand.h"
#include "Event.h"
#include "CollisionSystem.h"
#include "BoxColliderComponent.h"
#include "BlockComponent.h"
#include "WallComponent.h"
#include "Animator.h"
#include <iostream>

class PushCommand : public dae::GameObjectCommand
{
public:
	PushCommand(dae::GameObject* gameObject)
		:dae::GameObjectCommand(gameObject)
	{
		m_pAnimator = gameObject->GetComponent<Animator>();
		m_pBoxColliderComponent = gameObject->GetComponent<dae::BoxColliderComponent>();
		assert(m_pAnimator != nullptr && m_pBoxColliderComponent != nullptr);
	}

	virtual void Execute() override
	{
		if (m_pAnimator == nullptr || m_pBoxColliderComponent == nullptr) return;

		dae::Raycast raycast{};
		InitializeRaycast(raycast);

		dae::HitInfo hitInfo_Out{};
		if (!dae::CollisionSystem::GetInstance().PerformRaycast(raycast, hitInfo_Out, m_pBoxColliderComponent)) return;
		if (hitInfo_Out.other == nullptr) return;

		if (hitInfo_Out.other->GetLayer() == dae::Layer::block)
		{
			BlockComponent* blockComponent = hitInfo_Out.other->GetGameObject()->GetComponent<BlockComponent>();
			if (blockComponent == nullptr) return;

			blockComponent->Push(raycast.directionX, raycast.directionY);
		}

		if (hitInfo_Out.other->GetLayer() == dae::Layer::wall)
		{
			WallComponent* wallComponent = hitInfo_Out.other->GetGameObject()->GetComponent<WallComponent>();
			if (wallComponent == nullptr) return;

			wallComponent->Push();
		}
	}

private:
	Animator* m_pAnimator{ nullptr };
	dae::BoxColliderComponent* m_pBoxColliderComponent{ nullptr };
	float m_Distance{ 16.0f };

	void InitializeRaycast(dae::Raycast& raycast)
	{
		dae::BoxCollider boxCollider = m_pBoxColliderComponent->BoxDimensions();

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


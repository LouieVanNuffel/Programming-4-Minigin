#include "CollisionSystem.h"
#include "BoxColliderComponent.h"
#include <cassert>

using namespace dae;

CollisionSystem::CollisionSystem()
{
}

void CollisionSystem::AddCollider(BoxColliderComponent* boxColliderComponent)
{
	m_BoxColliderComponents.emplace_back(boxColliderComponent);
}

void CollisionSystem::RemoveCollider(BoxColliderComponent* boxColliderComponent)
{
	auto it = std::find(m_BoxColliderComponents.begin(), m_BoxColliderComponents.end(), boxColliderComponent);

	assert(it != m_BoxColliderComponents.end());
	if (it == m_BoxColliderComponents.end()) return;

	m_BoxColliderComponents.erase(it);
}

bool dae::CollisionSystem::PerformRaycast(const Raycast& raycast, HitInfo& hitInfo_Out)
{
	for (const BoxColliderComponent* boxColiderComponent : m_BoxColliderComponents)
	{
		const BoxCollider& boxCollider = boxColiderComponent->BoxDimensions();
		float currentX{};
		float currentY{};

		for (float currentDistance{}; currentDistance <= raycast.distance; currentDistance += 0.5f)
		{
			currentX = raycast.startX + (raycast.directionX * currentDistance);
			currentY = raycast.startY + (raycast.directionY * currentDistance);
			if (AreColliding(currentX, currentY, boxCollider))
			{
				hitInfo_Out.x = currentX;
				hitInfo_Out.y = currentY;
				hitInfo_Out.other = boxColiderComponent;
				return true;
			}
		}
	}

	return false;
}

void CollisionSystem::Update()
{
	for (uint32_t index1{}; index1 < m_BoxColliderComponents.size(); ++index1)
	{
		for (uint32_t index2{index1 + 1}; index2 < m_BoxColliderComponents.size(); ++index2)
		{
			if (m_BoxColliderComponents[index1] == m_BoxColliderComponents[index2]) continue;
			if (!MovedThisFrame(m_BoxColliderComponents[index1]) && !MovedThisFrame(m_BoxColliderComponents[index2])) continue;

			BoxCollider collider1 = m_BoxColliderComponents[index1]->BoxDimensions();
			BoxCollider collider2 = m_BoxColliderComponents[index2]->BoxDimensions();

			if (AreColliding(collider1, collider2))
			{
				m_BoxColliderComponents[index1]->CallOnCollisionEnter(*m_BoxColliderComponents[index2]);
				m_BoxColliderComponents[index2]->CallOnCollisionEnter(*m_BoxColliderComponents[index1]);
				MoveColliders(m_BoxColliderComponents[index1], m_BoxColliderComponents[index2]);
			}
		}
	}
}

bool dae::CollisionSystem::AreColliding(const BoxCollider& box1, const BoxCollider& box2) const
{
	if (DistanceX(box1, box2) <= (box1.width * 0.5f + box2.width * 0.5f)
		&& DistanceY(box1, box2) <= (box1.height * 0.5f + box2.height * 0.5f))
	{
		return true;
	}

	return false;
}

bool dae::CollisionSystem::AreColliding(float positionX, float positionY, const BoxCollider& box) const
{
	if (positionX >= box.x && positionX <= box.x + box.width
		&& positionY >= box.y && positionY <= box.y + box.height)
	{
		return true;
	}

	return false;
}

float dae::CollisionSystem::OverlapX(const BoxCollider& box1, const BoxCollider& box2) const
{
	float overlap = ((box1.width * 0.5f + box2.width * 0.5f)) - DistanceX(box1, box2);
	if (overlap <= 0) return 0;
	return overlap;
}

float dae::CollisionSystem::OverlapY(const BoxCollider& box1, const BoxCollider& box2) const
{
	float overlap = ((box1.height * 0.5f + box2.height * 0.5f)) - DistanceY(box1, box2);;
	if (overlap <= 0) return 0;
	return overlap;
}

int dae::CollisionSystem::DirectionX(const BoxCollider& box1, const BoxCollider& box2) const
{
	float direction = (box2.centerX - box1.centerX);
	if (direction == 0) return 0;
	return static_cast<int>(direction / fabsf(direction));
}

int dae::CollisionSystem::DirectionY(const BoxCollider& box1, const BoxCollider& box2) const
{
	float direction = (box2.centerY - box1.centerY);
	if (direction == 0) return 0;
	return static_cast<int>(direction / fabsf(direction));
}

float dae::CollisionSystem::DistanceX(const BoxCollider& box1, const BoxCollider& box2) const
{
	return fabsf(box1.centerX - box2.centerX);
}

float dae::CollisionSystem::DistanceY(const BoxCollider& box1, const BoxCollider& box2) const
{
	return fabsf(box1.centerY - box2.centerY);
}

bool dae::CollisionSystem::MovedThisFrame(const BoxColliderComponent* boxColliderComponent) const
{
	if (fabsf(boxColliderComponent->Velocity().x) < FLT_EPSILON
		&& fabsf(boxColliderComponent->Velocity().y) < FLT_EPSILON)
	{
		return false;
	}

	return true;
}

void dae::CollisionSystem::MoveColliders(BoxColliderComponent* boxColliderComponent1, BoxColliderComponent* boxColliderComponent2)
{
	ObjectType objectType1 = boxColliderComponent1->GetObjectType();
	ObjectType objectType2 = boxColliderComponent2->GetObjectType();

	if (objectType1 == ObjectType::immovable && objectType2 == ObjectType::immovable) return;

	BoxCollider collider1 = boxColliderComponent1->BoxDimensions();
	BoxCollider collider2 = boxColliderComponent2->BoxDimensions();

	float overlapX = OverlapX(collider1, collider2);
	float overlapY = OverlapY(collider1, collider2);

	// Only move on the axis of least penetration to avoid glitchyness
	if (overlapX < overlapY) overlapY = 0;
	else overlapX = 0;

	if (objectType1 == ObjectType::movable && objectType2 == ObjectType::immovable)
	{
		boxColliderComponent1->Move(DirectionX(collider2, collider1) * overlapX, 
									DirectionY(collider2, collider1) * overlapY, ObjectType::immovable);
		return;
	}

	if (objectType1 == ObjectType::immovable && objectType2 == ObjectType::movable)
	{
		boxColliderComponent2->Move(DirectionX(collider1, collider2) * overlapX, 
									DirectionY(collider1, collider2) * overlapY, ObjectType::immovable);
		return;
	}

	if (objectType1 == ObjectType::movable && objectType2 == ObjectType::movable)
	{
		boxColliderComponent1->Move(DirectionX(collider2, collider1) * overlapX * 0.5f, 
									DirectionY(collider2, collider1) * overlapY * 0.5f, ObjectType::movable);
		boxColliderComponent2->Move(DirectionX(collider1, collider2) * overlapX * 0.5f, 
									DirectionY(collider1, collider2) * overlapY * 0.5f, ObjectType::movable);
		return;
	}
}

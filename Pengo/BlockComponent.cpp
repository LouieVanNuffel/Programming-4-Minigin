#include "BlockComponent.h"
#include "BoxColliderComponent.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "EngineTime.h"
#include "CollisionSystem.h"

BlockComponent::BlockComponent(BlockType blockType, int blockSize, dae::GameObject* gameObject)
	:Component(gameObject), m_BlockType{ blockType }, m_BlockSize{ blockSize }
{
}

void BlockComponent::Start()
{
	m_pBoxColliderComponent = m_gameObject->GetComponent<dae::BoxColliderComponent>();
	assert(m_pBoxColliderComponent != nullptr);
}

void BlockComponent::Update()
{
	if (m_IsMoving) PerformRaycast();
	if (!m_IsMoving) return;

	glm::vec3 currentPosition = m_gameObject->GetTransform().GetPosition();
	float deltaTime = dae::Time::GetInstance().GetDeltaTime();
	m_gameObject->SetPosition(currentPosition.x + (m_DirectionX * m_Speed * deltaTime),
							  currentPosition.y + (m_DirectionY * m_Speed * deltaTime));
}

void BlockComponent::Push(int directionX, int directionY)
{
	if (directionX == 0 && directionY == 0) return;

	m_IsMoving = true;
	m_DirectionX = directionX;
	m_DirectionY = directionY;
}

void BlockComponent::PerformRaycast()
{
	if (m_pBoxColliderComponent == nullptr) return;

	dae::BoxCollider boxCollider = m_pBoxColliderComponent->BoxDimensions();

	dae::Raycast raycast{};
	raycast.startX = boxCollider.centerX;
	raycast.startY = boxCollider.centerY;
	raycast.directionX = m_DirectionX;
	raycast.directionY = m_DirectionY;
	raycast.distance = static_cast<float>(m_BlockSize);

	dae::HitInfo hitInfo_Out{};
	if (dae::CollisionSystem::GetInstance().PerformRaycast(raycast, hitInfo_Out, m_pBoxColliderComponent))
	{
		if (hitInfo_Out.other == nullptr) return;

		dae::HealthComponent* healthComponent = hitInfo_Out.other->GetGameObject()->GetComponent<dae::HealthComponent>();
		if (healthComponent != nullptr)
		{
			healthComponent->TakeDamage(100.0f);
		}
		else
		{
			m_IsMoving = false;
			if (hitInfo_Out.other->BoxDimensions().width <= m_BlockSize) SetPositionNextToBlock(hitInfo_Out.other);
			else SetPositionNextToWall(hitInfo_Out.x, hitInfo_Out.y);
		}
	}
}

void BlockComponent::SetPositionNextToBlock(const dae::BoxColliderComponent* other)
{
	dae::BoxCollider otherBoxCollider = other->BoxDimensions();
	dae::BoxCollider boxCollider = m_pBoxColliderComponent->BoxDimensions();

	float targetX = otherBoxCollider.centerX - m_DirectionX * m_BlockSize;
	float targetY = otherBoxCollider.centerY - m_DirectionY * m_BlockSize;

	targetX -= otherBoxCollider.width * 0.5f;
	targetY -= otherBoxCollider.height * 0.5f;

	m_gameObject->SetWorldPosition(targetX, targetY);
}

void BlockComponent::SetPositionNextToWall(float x, float y)
{
	float targetX = x - m_DirectionX * m_BlockSize * 0.5f;
	float targetY = y - m_DirectionY * m_BlockSize * 0.5f;

	targetX -= m_BlockSize * 0.5f;
	targetY -= m_BlockSize * 0.5f;

	m_gameObject->SetWorldPosition(targetX, targetY);
}

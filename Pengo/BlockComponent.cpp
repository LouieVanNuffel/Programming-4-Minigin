#include "BlockComponent.h"
#include "BoxColliderComponent.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "EngineTime.h"

BlockComponent::BlockComponent(BlockType blockType, dae::GameObject* gameObject)
	:Component(gameObject), m_BlockType{ blockType }
{
}

void BlockComponent::Update()
{
	if (!m_IsMoving) return;

	glm::vec3 currentPosition = m_gameObject->GetTransform().GetPosition();
	float deltaTime = dae::Time::GetInstance().GetDeltaTime();
	m_gameObject->SetPosition(currentPosition.x + (m_DirectionX * m_Speed * deltaTime),
							  currentPosition.y + (m_DirectionY * m_Speed * deltaTime));
}

void BlockComponent::OnCollisionEnter(const dae::BoxColliderComponent& other)
{
	if (!m_IsMoving) return;

	dae::HealthComponent* healthComponent = other.GetGameObject()->GetComponent<dae::HealthComponent>();
	if (healthComponent == nullptr) return;

	healthComponent->TakeDamage(100.0f);
}

void BlockComponent::Push(int directionX, int directionY)
{
	if (directionX == 0 && directionY == 0) return;

	m_IsMoving = true;
	m_DirectionX = directionX;
	m_DirectionY = directionY;
}

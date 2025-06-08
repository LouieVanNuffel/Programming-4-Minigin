#include "SnoBeeComponent.h"
#include "BoxColliderComponent.h"
#include "HealthComponent.h"
#include "EngineTime.h"
#include "CollisionSystem.h"
#include "BlockComponent.h"

SnoBeeComponent::SnoBeeComponent(dae::GameObject* gameObject, float blockSize, float speed, float chaseRange)
	:Component(gameObject), m_BlockSize{ blockSize }, m_Speed{ speed }, m_ChaseRange{ chaseRange }
{
	SetRandomDirection();
}

void SnoBeeComponent::Start()
{
	m_pBoxColliderComponent = m_gameObject->GetComponent<dae::BoxColliderComponent>();
	assert(m_pBoxColliderComponent != nullptr);
}

void SnoBeeComponent::Update()
{
	// Move in current direction with set speed
	float deltaTime = dae::Time::GetInstance().GetDeltaTime();
	glm::vec3 currentPosition = m_gameObject->GetTransform().GetPosition();
	m_gameObject->SetPosition(currentPosition.x + m_DirectionX * m_Speed * deltaTime, currentPosition.y + m_DirectionY * m_Speed * deltaTime);

	switch (m_CurrentBehaviorState)
	{
	case BehaviorState::patrol:
		Patrol();
		break;
	case BehaviorState::chase:
		Chase();
		break;
	}
}

void SnoBeeComponent::OnCollisionEnter(const dae::BoxColliderComponent& other)
{
	if (other.GetLayer() == dae::Layer::pengo)
	{
		dae::HealthComponent* healthComponent = other.GetGameObject()->GetComponent<dae::HealthComponent>();
		if (healthComponent == nullptr) return;

		healthComponent->TakeDamage(100.0f);
	}
}

void SnoBeeComponent::SetDirection(MoveDirection direction)
{
	m_Direction = direction;

	switch (m_Direction)
	{
	case MoveDirection::up:
		m_DirectionX = 0;
		m_DirectionY = -1;
		break;
	case MoveDirection::down:
		m_DirectionX = 0;
		m_DirectionY = 1;
		break;
	case MoveDirection::left:
		m_DirectionX = -1;
		m_DirectionY = 0;
		break;
	case MoveDirection::right:
		m_DirectionX = 1;
		m_DirectionY = 0;
		break;
	}
}

void SnoBeeComponent::SetRandomDirection()
{
	int randomInt = std::rand() % 4;
	SetDirection(static_cast<MoveDirection>(randomInt));
}

bool SnoBeeComponent::PerformRaycast(dae::HitInfo& hitInfo_Out) const
{
	if (m_pBoxColliderComponent == nullptr) return false;

	dae::BoxCollider boxCollider = m_pBoxColliderComponent->BoxDimensions();

	dae::Raycast raycast{};
	raycast.startX = boxCollider.centerX;
	raycast.startY = boxCollider.centerY;
	raycast.directionX = m_DirectionX;
	raycast.directionY = m_DirectionY;
	raycast.distance = m_BlockSize * 0.5f;

	if (dae::CollisionSystem::GetInstance().PerformRaycast(raycast, hitInfo_Out, m_pBoxColliderComponent))
	{
		return true;
	}

	return false;
}

void SnoBeeComponent::Patrol()
{
	dae::HitInfo hitInfo_Out{};
	if (PerformRaycast(hitInfo_Out))
	{
		if (hitInfo_Out.other == nullptr) return;
		if (hitInfo_Out.other->GetLayer() == dae::Layer::pengo)
		{
			m_CurrentBehaviorState = BehaviorState::chase;
			m_pTargetObject = hitInfo_Out.other->GetGameObject();
			return;
		}

		SetRandomDirection();
	}
}

void SnoBeeComponent::Chase()
{
	if (m_pTargetObject == nullptr)
	{
		m_CurrentBehaviorState = BehaviorState::patrol;
		return;
	}

	// Check if target still within range
	if (DistanceToTargetSquared() >= m_ChaseRange * m_ChaseRange)
	{
		m_pTargetObject = nullptr;
		return;
	}

	// Set direction to target
	glm::vec2 direction = DirectionToTarget();
	m_DirectionX = static_cast<int>(direction.x);
	m_DirectionY = static_cast<int>(direction.y);

	// Destroy blocks in path
	dae::HitInfo hitInfo_Out{};
	if (PerformRaycast(hitInfo_Out))
	{
		if (hitInfo_Out.other == nullptr) return;
		if (hitInfo_Out.other->GetLayer() == dae::Layer::block)
		{
			BlockComponent* blockComponent = hitInfo_Out.other->GetGameObject()->GetComponent<BlockComponent>();
			if (blockComponent == nullptr) return;
			blockComponent->Break();
		}
	}
}

float SnoBeeComponent::DistanceToTargetSquared() const
{
	if (m_pTargetObject == nullptr) return 0.0f;

	glm::vec3 targetPosition = m_pTargetObject->GetWorldPosition();
	glm::vec3 currentPosition = m_gameObject->GetWorldPosition();
	float distanceX = fabsf(targetPosition.x - currentPosition.x);
	float distanceY = fabsf(targetPosition.y - currentPosition.y);
	float totalDistanceSquared = powf(distanceX, 2.0f) + powf(distanceY, 2.0f);
	return totalDistanceSquared;
}

glm::vec2 SnoBeeComponent::DirectionToTarget() const
{
	float toTargetX = m_pTargetObject->GetWorldPosition().x - m_gameObject->GetWorldPosition().x;
	float toTargetY = m_pTargetObject->GetWorldPosition().y - m_gameObject->GetWorldPosition().y;

	glm::vec2 directionToTarget{};

	// Avoid division by zero
	if (toTargetX == 0.0f && toTargetY == 0.0f) return glm::vec2{};

	// returns in the direction where the target is still the furthest away
	if (fabsf(toTargetX) >= fabsf(toTargetY)) directionToTarget = glm::vec2{ fabsf(toTargetX) / toTargetX, 0.0f };
	else directionToTarget = glm::vec2{ 0.0f, fabsf(toTargetY) / toTargetY };

	return directionToTarget;
}

#include "SnoBeeComponent.h"
#include "BoxColliderComponent.h"
#include "HealthComponent.h"
#include "EngineTime.h"
#include "CollisionSystem.h"
#include "BlockComponent.h"
#include "MoveCommand.h"
#include "Event.h"
#include "LevelState.h"
#include "Subject.h"

SnoBeeComponent::SnoBeeComponent(dae::GameObject* gameObject, float blockSize, float speed, float chaseRange, float spawnDelay)
	:Component(gameObject), m_BlockSize{ blockSize }, m_Speed{ speed }, m_ChaseRange{ chaseRange }, m_SpawnTimer{ spawnDelay }
{
	SetRandomDirection();
	LevelState::GetInstance().AddSnoBee();
}

SnoBeeComponent::~SnoBeeComponent()
{
	LevelState::GetInstance().RemoveSnoBee(m_gameObject->IsActive());
}

void SnoBeeComponent::Start()
{
	m_pBoxColliderComponent = m_gameObject->GetComponent<dae::BoxColliderComponent>();
	assert(m_pBoxColliderComponent != nullptr);

	m_pSubjectComponent = m_gameObject->GetComponent<dae::Subject>();
}

void SnoBeeComponent::Update()
{
	if (!m_HasSpawned)
	{
		m_SpawnTimer -= dae::Time::GetInstance().GetDeltaTime();
		if (m_SpawnTimer <= 0.0f) m_HasSpawned = true;
		else return;
	}

	if (m_IsStunned)
	{
		m_StunTimer -= dae::Time::GetInstance().GetDeltaTime();
		if (m_StunTimer <= 0.0f) m_IsStunned = false;
		else return;
	}

	// Move in current direction
	MoveCommand moveCommand{ m_gameObject, static_cast<float>(m_DirectionX), static_cast<float>(m_DirectionY), m_Speed };
	moveCommand.Execute();

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

void SnoBeeComponent::Stun()
{
	m_IsStunned = true;
	m_StunTimer = m_StunTime;

	if (m_pSubjectComponent == nullptr) return;
	m_pSubjectComponent->NotifyObservers(dae::Event{ dae::make_sdbm_hash("Stunned") });
}

void SnoBeeComponent::Notify(const dae::Event& event, const dae::GameObject*)
{
	if (event.id == dae::make_sdbm_hash("Died"))
	{
		m_gameObject->Destroy();
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
	// Check for blocks or walls
	dae::HitInfo hitInfo_Out{};
	if (PerformRaycast(hitInfo_Out))
	{
		if (hitInfo_Out.other == nullptr) return;

		if (hitInfo_Out.other->GetLayer() == dae::Layer::block)
		{
			srand(static_cast<int>(time(NULL)));
			int randomNumber = rand() % 2;
			if (randomNumber == 0)
			{
				SetRandomDirection();
				return;
			}
			else
			{
				BlockComponent* blockComponent = hitInfo_Out.other->GetGameObject()->GetComponent<BlockComponent>();
				if (blockComponent != nullptr)
				{
					blockComponent->Break();
					return;
				}
			}
		}

		SetRandomDirection();
	}

	// Check if a player is close (if multiple are close go after the closest one)
	for (const auto& playerObject : LevelState::GetInstance().GetPlayerObjects())
	{
		glm::vec3 playerPosition = playerObject->GetWorldPosition();
		float distance = DistanceToObjectSquared(playerObject);
		if (distance <= m_ChaseRange * m_ChaseRange)
		{
			if (m_pTargetObject != nullptr)
			{
				if (distance < DistanceToObjectSquared(m_pTargetObject)) m_pTargetObject = playerObject;
			}
			else m_pTargetObject = playerObject;

			m_CurrentBehaviorState = BehaviorState::chase;
		}
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
	if (DistanceToObjectSquared(m_pTargetObject) >= m_ChaseRange * m_ChaseRange)
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

float SnoBeeComponent::DistanceToObjectSquared(dae::GameObject* gameObject) const
{
	if (gameObject == nullptr) return 0.0f;

	glm::vec3 targetPosition = gameObject->GetWorldPosition();
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

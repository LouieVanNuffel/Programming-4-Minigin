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
#include "AIControllerComponent.h"
#include "Layers.h"
#include "SceneManager.h"
#include "Scene.h"
#include <random>

SnoBeeComponent::SnoBeeComponent(dae::GameObject* gameObject, float blockSize, float speed, float chaseRange, float spawnDelay)
	:Component(gameObject), m_BlockSize{ blockSize }, m_Speed{ speed }, m_ChaseRange{ chaseRange }, m_SpawnTimer{ spawnDelay }
{
	SetRandomDirection();
	dae::SceneManager::GetInstance().ActiveScene().levelState->AddSnoBee(this);
}

void SnoBeeComponent::OnDestroy()
{
	dae::SceneManager::GetInstance().ActiveScene().levelState->RemoveSnoBee(this);
}

void SnoBeeComponent::Start()
{
	m_pBoxColliderComponent = m_gameObject->GetComponent<dae::BoxColliderComponent>();
	assert(m_pBoxColliderComponent != nullptr);

	m_pAIControllerComponent = m_gameObject->GetComponent<AIControllerComponent>();
	assert(m_pAIControllerComponent != nullptr);

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

	// Move in current direction (order of direction and action matches up)
	m_pAIControllerComponent->ExecuteAction(static_cast<uint32_t>(m_Direction));

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
	if (other.GetLayer() == static_cast<uint32_t>(Layer::pengo))
	{
		if (!m_IsStunned)
		{
			dae::HealthComponent* healthComponent = other.GetGameObject()->GetComponent<dae::HealthComponent>();
			if (healthComponent == nullptr) return;

			healthComponent->TakeDamage(100.0f);
		}
		else
		{
			// Die
			m_gameObject->Destroy();
		}
	}
}

void SnoBeeComponent::Stun()
{
	if (m_IsStunned) return;

	m_IsStunned = true;
	m_StunTimer = m_StunTime;

	if (m_pSubjectComponent == nullptr) return;
	m_pSubjectComponent->NotifyObservers(dae::Event{ dae::make_sdbm_hash("Stunned") });
}

bool SnoBeeComponent::Hatched() const
{
	return m_gameObject->IsActive();
}

void SnoBeeComponent::Notify(const dae::Event& event, const dae::GameObject*)
{
	if (event.id == dae::make_sdbm_hash("Died"))
	{
		m_gameObject->Destroy();
	}

	if (event.id == dae::make_sdbm_hash("Respawn"))
	{
		Respawn();
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

		if (hitInfo_Out.other->GetLayer() == static_cast<uint32_t>(Layer::block))
		{
			std::mt19937 rng{ std::random_device{}() };
			std::uniform_int_distribution<int> dist(0, 1);
			int randomNumber = dist(rng);
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
	for (const auto& playerObject : dae::SceneManager::GetInstance().ActiveScene().levelState->GetPlayerObjects())
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
	SetDirection(DirectionToTarget());

	// Destroy blocks in path
	dae::HitInfo hitInfo_Out{};
	if (PerformRaycast(hitInfo_Out))
	{
		if (hitInfo_Out.other == nullptr) return;
		if (hitInfo_Out.other->GetLayer() == static_cast<uint32_t>(Layer::block))
		{
			BlockComponent* blockComponent = hitInfo_Out.other->GetGameObject()->GetComponent<BlockComponent>();
			if (blockComponent == nullptr) return;
			blockComponent->Break();
		}
	}
}

void SnoBeeComponent::Respawn()
{
	glm::vec3 randomCorner = dae::SceneManager::GetInstance().ActiveScene().levelState->GetRandomCornerTilePosition();
	m_gameObject->SetWorldPosition(randomCorner.x, randomCorner.y);
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

SnoBeeComponent::MoveDirection SnoBeeComponent::DirectionToTarget() const
{
	return DirectionToPosition(m_pTargetObject->GetWorldPosition());
}

SnoBeeComponent::MoveDirection SnoBeeComponent::DirectionToPosition(const glm::vec3& position) const
{
	float toPositionX = position.x - m_gameObject->GetWorldPosition().x;
	float toPositionY = position.y - m_gameObject->GetWorldPosition().y;

	// Avoid division by zero
	if (toPositionX == 0.0f && toPositionY == 0.0f) return m_Direction;

	// returns in the direction where the target is still the furthest away
	if (fabsf(toPositionX) >= fabsf(toPositionY))
	{
		float directionToTarget = fabsf(toPositionX) / toPositionX;
		if (directionToTarget <= 0.0f) return MoveDirection::left;
		else return MoveDirection::right;
	}
	else
	{
		float directionToTarget = fabsf(toPositionY) / toPositionY;
		if (directionToTarget <= 0.0f) return MoveDirection::up;
		else return MoveDirection::down;
	}
}

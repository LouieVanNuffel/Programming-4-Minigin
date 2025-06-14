#include "BlockComponent.h"
#include "BoxColliderComponent.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "EngineTime.h"
#include "CollisionSystem.h"
#include "RenderComponent.h"
#include "SceneManager.h"
#include "SnoBeeCharacter.h"
#include "LevelState.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Layers.h"

BlockComponent::BlockComponent(BlockType blockType, int blockSize, dae::GameObject* gameObject)
	:Component(gameObject), m_BlockType{ blockType }, m_BlockSize{ blockSize }
{
	if (m_BlockType == BlockType::egg)
	{
		dae::SceneManager::GetInstance().ActiveScene().levelState->RegisterEggBlock(this);
		InitializeSnoBeeCharacter();
	}
}

void BlockComponent::OnDestroy()
{
	if (m_BlockType == BlockType::egg)
	{
		dae::SceneManager::GetInstance().ActiveScene().levelState->UnregisterEggBlock(this);
		// Destroy the snoBeeCharacter as well if the egg hasn't hatched yet
		if (m_SnoBeeCharacter->IsActive() == false)
		{
			m_SnoBeeCharacter->Destroy();
		}
	}
}

void BlockComponent::Start()
{
	m_pBoxColliderComponent = m_gameObject->GetComponent<dae::BoxColliderComponent>();
	m_pRenderComponent = m_gameObject->GetComponent<dae::RenderComponent>();
	assert(m_pBoxColliderComponent != nullptr && m_pRenderComponent != nullptr);
}

void BlockComponent::Update()
{
	if (m_IsBroken && m_pRenderComponent != nullptr) UpdateBreakAnimation();

	if (!m_IsMoving) return;

	if (PerformRaycast()) m_IsMoving = false;
	else
	{
		glm::vec3 currentPosition = m_gameObject->GetTransform().GetPosition();
		float deltaTime = dae::Time::GetInstance().GetDeltaTime();
		m_gameObject->SetPosition(currentPosition.x + (m_DirectionX * m_Speed * deltaTime),
			currentPosition.y + (m_DirectionY * m_Speed * deltaTime));
	}
}

void BlockComponent::Push(int directionX, int directionY)
{
	if (m_IsBroken) return;
	if (directionX == 0 && directionY == 0) return;

	m_DirectionX = directionX;
	m_DirectionY = directionY;

	// If there is no empty space in the direction the block gets pushed, break the block
	if (PerformRaycast()) Break();
	else m_IsMoving = true;
}

void BlockComponent::Break()
{
	// The diamond cannot break
	if (m_BlockType == BlockType::diamond) return;

	m_IsBroken = true;
}

void BlockComponent::Hatch()
{
	if (m_BlockType != BlockType::egg) return;
	// Activate SnobeeCharacter
	m_SnoBeeCharacter->SetActive(true);

	// Destroy block
	m_gameObject->Destroy();
}

bool BlockComponent::PerformRaycast()
{
	if (m_pBoxColliderComponent == nullptr) return false;

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
		if (hitInfo_Out.other == nullptr) return false;

		if (hitInfo_Out.other->GetLayer() == static_cast<uint32_t>(Layer::snobee) 
			|| hitInfo_Out.other->GetLayer() == static_cast<uint32_t>(Layer::pengo))
		{
			dae::HealthComponent* healthComponent = hitInfo_Out.other->GetGameObject()->GetComponent<dae::HealthComponent>();
			if (healthComponent != nullptr)
			{
				healthComponent->TakeDamage(100.0f);
				return false; // returns false because the block shouldn't stop when it hits a damageable object
			}
		}
		else
		{
			if (hitInfo_Out.other->BoxDimensions().width <= m_BlockSize) SetPositionNextToBlock(hitInfo_Out.other);
			else SetPositionNextToWall(hitInfo_Out.x, hitInfo_Out.y);
			return true;
		}
	}

	return false;
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

void BlockComponent::UpdateBreakAnimation()
{
	m_BreakAnimationSequence.Update();
	m_pRenderComponent->AddSourceRectToStartPosition(m_BreakAnimationSequence.GetOffsetX(),
		m_BreakAnimationSequence.GetOffsetY());

	if (m_BreakAnimationSequence.Finished())
	{
		m_gameObject->Destroy();
	}
}

void BlockComponent::InitializeSnoBeeCharacter()
{
	// Initialize snobeecharacter
	SnoBeeCharacter snoBeeCharacter{ SnoBeeColor::green };
	const glm::vec3& currentPosition = m_gameObject->GetWorldPosition();
	m_SnoBeeCharacter = snoBeeCharacter.GetCharacterObject();
	m_SnoBeeCharacter->SetWorldPosition(currentPosition.x, currentPosition.y);
	m_SnoBeeCharacter->SetActive(false);

	// Spawn snobeecharacter
	auto& sceneManager = dae::SceneManager::GetInstance();
	sceneManager.AddGameObjectToScene(snoBeeCharacter.GetCharacterObject(), sceneManager.ActiveSceneIndex());
}

#include "BoxColliderComponent.h"
#include "GameObject.h"
#include "CollisionSystem.h"
#include <iostream>
#include "VelocityComponent.h"

dae::BoxColliderComponent::BoxColliderComponent(float width, float height, ObjectType objectType, GameObject* gameObject)
	:Component(gameObject), m_ObjectType{ objectType }
{
	m_BoxDimensions.width = width;
	m_BoxDimensions.height = height;

	CollisionSystem::GetInstance().AddCollider(this);
}

void dae::BoxColliderComponent::Start()
{
	UpdatePosition();

	m_pVelocityComponent = m_gameObject->GetComponent<VelocityComponent>();
}

void dae::BoxColliderComponent::Update()
{
	UpdatePosition();
}

void dae::BoxColliderComponent::Move(float x, float y, ObjectType objectType)
{
	glm::vec3 currentPosition = m_gameObject->GetTransform().GetPosition();
	m_gameObject->SetPosition(currentPosition.x + x, currentPosition.y + y);
	UpdatePosition();

	if (objectType == ObjectType::immovable) m_pVelocityComponent->AddCollisionsOffset(x, y);
}

void dae::BoxColliderComponent::CallOnCollisionEnter(const BoxColliderComponent& other)
{
	m_gameObject->OnCollisionEnter(other);
}

const dae::BoxCollider& dae::BoxColliderComponent::BoxDimensions() const
{
	return m_BoxDimensions;
}

const dae::ObjectType& dae::BoxColliderComponent::GetObjectType() const
{
	return m_ObjectType;
}

const glm::vec3 dae::BoxColliderComponent::Velocity() const
{
	if (m_pVelocityComponent != nullptr) return m_pVelocityComponent->GetVelocity();
	return glm::vec3{};
}

const dae::GameObject* dae::BoxColliderComponent::GetGameObject() const
{
	return m_gameObject;
}

void dae::BoxColliderComponent::UpdatePosition()
{
	glm::vec3 gameObjectPosition = m_gameObject->GetWorldPosition();
	m_BoxDimensions.x = gameObjectPosition.x;
	m_BoxDimensions.y = gameObjectPosition.y;
	m_BoxDimensions.centerX = m_BoxDimensions.x + (m_BoxDimensions.width * 0.5f);
	m_BoxDimensions.centerY = m_BoxDimensions.y + (m_BoxDimensions.height * 0.5f);
}

#include "TileMovementComponent.h"
#include "LevelState.h"
#include "GameObject.h"
#include "EngineTime.h"
#include <iostream>

TileMovementComponent::TileMovementComponent(dae::GameObject* gameObject, float speed)
	:dae::Component(gameObject), m_Speed{ speed }
{

}

void TileMovementComponent::Update()
{
	if (m_HasArrived) return;

	glm::vec3 currentPosition = m_gameObject->GetWorldPosition();
	glm::vec3 directionVector = m_TargetPosition - currentPosition;
	float deltaTime = dae::Time::GetInstance().GetDeltaTime();

	Normalize(directionVector);
	m_gameObject->SetWorldPosition(currentPosition.x + directionVector.x * m_Speed * deltaTime,
								   currentPosition.y + directionVector.y * m_Speed * deltaTime);
}

void TileMovementComponent::SetDirection(float directionX, float directionY)
{
	m_HasArrived = false;
	glm::vec3 currentPosition = m_gameObject->GetWorldPosition();
	m_TargetPosition = LevelState::GetInstance().GetClosestTilePositionInDirection(
		currentPosition, directionX, directionY);
}

void TileMovementComponent::Normalize(glm::vec3& vector)
{
	float length = sqrtf(powf(vector.x, 2.f) + powf(vector.y, 2.f) + powf(vector.z, 2.f));
	if (length < 1.f)
	{
		vector = glm::vec3{};
		m_HasArrived = true;
		return;
	}

	vector /= length;
}

#include "RotationComponent.h"
#include "GameObject.h"

dae::RotationComponent::RotationComponent(GameObject* gameObject, float rotationSpeedPerSecond, float rotationCircleRadius)
	:Component(gameObject), m_rotationSpeedPerSecond{ rotationSpeedPerSecond }, m_rotationCircleRadius{ rotationCircleRadius }
{
}

void dae::RotationComponent::Start(){}

void dae::RotationComponent::Update(float deltaTime)
{
	m_currentRotation += m_rotationSpeedPerSecond * deltaTime;
	glm::vec3 rotatedPos{};
	rotatedPos.x = m_rotationCircleRadius * cosf(m_currentRotation);
	rotatedPos.y = m_rotationCircleRadius * sinf(m_currentRotation);
	m_gameObject->SetLocalPosition(rotatedPos);
}

void dae::RotationComponent::LateUpdate(float){}

void dae::RotationComponent::Render() const{}

void dae::RotationComponent::RenderUI() const{}

#include "MoveComponent.h"
#include "GameObject.h"

dae::MoveComponent::MoveComponent(GameObject* gameObject, float speed, bool isContinuous)
	:Component(gameObject), m_Speed{ speed }, m_IsContinuous{ isContinuous }
{
}

dae::MoveComponent::~MoveComponent() {}

void dae::MoveComponent::Start() {}

void dae::MoveComponent::Update(float deltaTime) 
{
	m_DeltaTime = deltaTime;

	if (m_IsContinuous)
	{
		float directionX = -static_cast<float>(m_Left) + static_cast<float>(m_Right);
		float directionY = -static_cast<float>(m_Up) + static_cast<float>(m_Down);

		glm::vec3 currentPosition{ m_gameObject->GetTransform().GetPosition() };
		m_gameObject->SetPosition(currentPosition.x + directionX * m_Speed * m_DeltaTime,
			currentPosition.y + directionY * m_Speed * m_DeltaTime);
	}
}

void dae::MoveComponent::LateUpdate(float) {}

void dae::MoveComponent::Render() const {}

void dae::MoveComponent::RenderUI() const {}

void dae::MoveComponent::Move(float directionX, float directionY)
{
	glm::vec3 currentPosition{ m_gameObject->GetTransform().GetPosition() };
	m_gameObject->SetPosition(currentPosition.x + directionX * m_Speed * m_DeltaTime,
		currentPosition.y + directionY * m_Speed * m_DeltaTime);
}

void dae::MoveComponent::SetUpPressed(bool value)
{
	m_Up = value;
}

void dae::MoveComponent::SetDownPressed(bool value)
{
	m_Down = value;
}

void dae::MoveComponent::SetLeftPressed(bool value)
{
	m_Left = value;
}

void dae::MoveComponent::SetRightPressed(bool value)
{
	m_Right = value;
}

#include "VelocityComponent.h"
#include "GameObject.h"
#include "Subject.h"
#include "Event.h"

#include <iostream>

dae::VelocityComponent::VelocityComponent(GameObject* gameObject)
	:Component(gameObject)
{

}

dae::VelocityComponent::~VelocityComponent()
{
}

void dae::VelocityComponent::Start()
{
	m_PositionThisFrame = m_gameObject->GetWorldPosition();
	m_PositionLastFrame = m_PositionThisFrame;

	m_pSubject = m_gameObject->GetComponent<Subject>();
}

void dae::VelocityComponent::Update()
{
}

// Late Update to make sure we take into account all changes this frame
void dae::VelocityComponent::LateUpdate()
{
	m_DirectionLastFrame = m_Direction;

	m_PositionLastFrame = m_PositionThisFrame;
	m_PositionThisFrame = m_gameObject->GetWorldPosition();
	m_Velocity = m_PositionThisFrame - m_PositionLastFrame - m_CollisionsOffset;
	// reset collisions offset once it's been applied
	m_CollisionsOffset.x = 0;
	m_CollisionsOffset.y = 0;

	// If didn't move at all
	if (fabsf(m_Velocity.x) < FLT_EPSILON && fabsf(m_Velocity.y) < FLT_EPSILON && fabsf(m_Velocity.z) < FLT_EPSILON)
	{
		m_Direction = Direction::none;
		NotifyAboutDirection(); // notify about no movement
		return;
	}

	// If moved set correct direction
	if (fabsf(m_Velocity.x) >= fabsf(m_Velocity.y))
	{
		if (m_Velocity.x < 0) m_Direction = Direction::left;
		else m_Direction = Direction::right;
	}
	else
	{
		if (m_Velocity.y < 0) m_Direction = Direction::up;
		else m_Direction = Direction::down;
	}

	// Notify about direction
	NotifyAboutDirection();
}

void dae::VelocityComponent::Render() const
{
}

void dae::VelocityComponent::RenderUI() const
{
}

const glm::vec3& dae::VelocityComponent::GetVelocity() const
{
	return m_Velocity;
}

void dae::VelocityComponent::AddCollisionsOffset(float x, float y)
{
	m_CollisionsOffset.x += x;
	m_CollisionsOffset.y += y;
}

void dae::VelocityComponent::NotifyAboutDirection() const
{
	if (m_Direction == m_DirectionLastFrame || m_pSubject == nullptr) return;

	if (m_Direction == Direction::none)
	{
		m_pSubject->NotifyObservers(Event{ make_sdbm_hash("DidNotMove") });
		return;
	}

	m_pSubject->NotifyObservers(Event{ make_sdbm_hash("Moved") });
	switch (m_Direction)
	{
		case Direction::up:
			m_pSubject->NotifyObservers(Event{ make_sdbm_hash("MovedUp") });
			break;
		case Direction::down:
			m_pSubject->NotifyObservers(Event{ make_sdbm_hash("MovedDown") });
			break;
		case Direction::left:
			m_pSubject->NotifyObservers(Event{ make_sdbm_hash("MovedLeft") });
			break;
		case Direction::right:
			m_pSubject->NotifyObservers(Event{ make_sdbm_hash("MovedRight") });
			break;
	}
}

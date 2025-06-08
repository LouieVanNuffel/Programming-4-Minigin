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
	m_PositionLastFrame = m_PositionThisFrame;
	m_PositionThisFrame = m_gameObject->GetWorldPosition();
	m_Velocity = m_PositionThisFrame - m_PositionLastFrame - m_CollisionsOffset;
	// reset collisions offset once it's been applied
	m_CollisionsOffset.x = 0;
	m_CollisionsOffset.y = 0;

	//std::cout << m_Velocity.x << ", " << m_Velocity.y << ", " << m_Velocity.z << std::endl;

	// Notify observers if applicable
	if (m_pSubject == nullptr) return;

	if (fabsf(m_Velocity.x) < FLT_EPSILON && fabsf(m_Velocity.y) < FLT_EPSILON && fabsf(m_Velocity.z) < FLT_EPSILON)
	{
		m_pSubject->NotifyObservers(Event{ make_sdbm_hash("DidNotMove") });
		return;
	}
	
	m_pSubject->NotifyObservers(Event{ make_sdbm_hash("Moved") });

	if (fabsf(m_Velocity.x) > FLT_EPSILON)
	{
		if (m_Velocity.x < 0) m_pSubject->NotifyObservers(Event{ make_sdbm_hash("MovedLeft") });
		else m_pSubject->NotifyObservers(Event{ make_sdbm_hash("MovedRight") });
	}

	if (fabsf(m_Velocity.y) > FLT_EPSILON)
	{
		if (m_Velocity.y < 0) m_pSubject->NotifyObservers(Event{ make_sdbm_hash("MovedUp") });
		else m_pSubject->NotifyObservers(Event{ make_sdbm_hash("MovedDown") });
	}
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

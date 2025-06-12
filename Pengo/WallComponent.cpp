#include "WallComponent.h"
#include "EngineTime.h"
#include "BoxColliderComponent.h"
#include "SnoBeeComponent.h"
#include "GameObject.h"
#include "Layers.h"

void WallComponent::Update()
{
	if (!m_IsVibrating) return;

	m_Timer -= dae::Time::GetInstance().GetDeltaTime();
	if (m_Timer <= 0.0f)
	{
		m_IsVibrating = false;
	}
}

void WallComponent::OnCollisionEnter(const dae::BoxColliderComponent& other)
{
	if (!m_IsVibrating) return;

	if (other.GetLayer() == static_cast<uint32_t>(Layer::snobee))
	{
		SnoBeeComponent* snoBeeComponent = other.GetGameObject()->GetComponent<SnoBeeComponent>();
		if (snoBeeComponent == nullptr) return;

		snoBeeComponent->Stun();
	}
}

void WallComponent::Push()
{
	m_IsVibrating = true;
	m_Timer = m_VibrationTime;
}

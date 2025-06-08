#include "SnoBeeComponent.h"
#include "BoxColliderComponent.h"
#include "HealthComponent.h"

SnoBeeComponent::SnoBeeComponent(dae::GameObject* gameObject)
	:Component(gameObject)
{
}

void SnoBeeComponent::Start()
{
}

void SnoBeeComponent::Update()
{
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

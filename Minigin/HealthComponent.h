#pragma once
#include "Component.h"
#include "Event.h"
#include "GameObject.h"

namespace dae
{
	class HealthComponent : public Component
	{
	public:
		HealthComponent(GameObject* gameObject)
			: Component(gameObject)
		{
		}

		virtual ~HealthComponent() = default;
		HealthComponent(const HealthComponent& other) = delete;
		HealthComponent(HealthComponent&& other) = delete;
		HealthComponent& operator=(const HealthComponent& other) = delete;
		HealthComponent& operator=(HealthComponent&& other) = delete;

		virtual void Start() override {};
		virtual void Update() override {};
		virtual void LateUpdate() override {};
		virtual void Render() const override {};
		virtual void RenderUI() const override {};

		float GetHealth() { return m_Health; }
		int GetLives() { return m_Lives; }
		void TakeDamage(float damage)
		{
			m_Health -= damage;
			if (m_Health <= 0.f)
			{
				m_Health = 100.f;
				--m_Lives;
				m_gameObject->NotifyObservers(Event{ make_sdbm_hash("PlayerDied") });
			}
		}

	private:
		float m_Health{ 100.f };
		int m_Lives{ 3 };
	};
}


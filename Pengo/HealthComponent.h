#pragma once
#include "Component.h"
#include "Event.h"
#include "GameObject.h"
#include "Subject.h"
#include "EngineTime.h"

namespace dae
{
	class HealthComponent : public Component
	{
	public:
		HealthComponent(GameObject* gameObject, int amountOfLives)
			: Component(gameObject), m_Lives{ amountOfLives }
		{
		}

		virtual ~HealthComponent() = default;
		HealthComponent(const HealthComponent& other) = delete;
		HealthComponent(HealthComponent&& other) = delete;
		HealthComponent& operator=(const HealthComponent& other) = delete;
		HealthComponent& operator=(HealthComponent&& other) = delete;

		virtual void Start() override
		{
			m_Subject = m_gameObject->GetComponent<Subject>();
		}

		virtual void Update() override
		{
			if (m_Timer <= 0.0f) return;

			m_Timer -= dae::Time::GetInstance().GetDeltaTime();
			if (m_Timer <= 0.0f) m_OnCooldown = false;
		};

		virtual void LateUpdate() override {};
		virtual void Render() const override {};
		virtual void RenderUI() const override {};

		float GetHealth() { return m_Health; }
		int GetLives() { return m_Lives; }
		void TakeDamage(float damage)
		{
			if (m_OnCooldown) return;

			m_OnCooldown = true;
			m_Timer = m_DamageCooldownTime;

			m_Health -= damage;
			if (m_Health <= 0.f)
			{
				m_Health = 100.f;
				--m_Lives;
				if (m_Subject != nullptr && m_Lives <= 0) m_Subject->NotifyObservers(Event{ make_sdbm_hash("Died") });
			}

			if (m_Subject != nullptr) m_Subject->NotifyObservers(Event{ make_sdbm_hash("TookDamage") });
		}

	private:
		float m_Health{ 100.f };
		float m_DamageCooldownTime{ 1.0f };
		float m_Timer{};
		int m_Lives;
		bool m_OnCooldown{ false };
		Subject* m_Subject{ nullptr };
	};
}


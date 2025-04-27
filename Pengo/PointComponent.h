#pragma once
#include "Component.h"
#include "Event.h"
#include "GameObject.h"
#include "Subject.h"

namespace dae
{
	class PointComponent : public Component
	{
	public:
		PointComponent(GameObject* gameObject)
			: Component(gameObject)
		{
		}

		virtual ~PointComponent() = default;
		PointComponent(const PointComponent& other) = delete;
		PointComponent(PointComponent&& other) = delete;
		PointComponent& operator=(const PointComponent& other) = delete;
		PointComponent& operator=(PointComponent&& other) = delete;

		virtual void Start() override
		{
			m_Subject = m_gameObject->GetComponent<Subject>();
		}

		virtual void Update() override {};
		virtual void LateUpdate() override {};
		virtual void Render() const override {};
		virtual void RenderUI() const override {};

		int GetPoints() { return m_Points; }
		void PickUpPoints(int amountOfPoints)
		{
			m_Points += amountOfPoints;
			if (m_Subject != nullptr) m_Subject->NotifyObservers(Event{ make_sdbm_hash("PlayerScored") });
		}

	private:
		int m_Points{};
		Subject* m_Subject;
	};
}


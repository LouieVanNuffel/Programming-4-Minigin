#pragma once
#include "Component.h"
#include "Event.h"
#include "GameObject.h"

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

		virtual void Start() override {};
		virtual void Update() override {};
		virtual void LateUpdate() override {};
		virtual void Render() const override {};
		virtual void RenderUI() const override {};

		int GetPoints() { return m_Points; }
		void PickUpPoints(int amountOfPoints)
		{
			m_Points += amountOfPoints;
			m_gameObject->NotifyObservers(Event{ make_sdbm_hash("PlayerScored") });
			if (m_Points >= 500) m_gameObject->NotifyObservers(Event{ make_sdbm_hash("GameWon") });
		}

	private:
		int m_Points{};
	};
}


#pragma once
#include "Component.h"
#include "Event.h"
#include "GameObject.h"
#include "Subject.h"

class PointComponent : public dae::Component
{
public:
	PointComponent(dae::GameObject* gameObject)
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
		m_Subject = m_gameObject->GetComponent<dae::Subject>();
	}

	virtual void Update() override {};
	virtual void LateUpdate() override {};
	virtual void Render() const override {};
	virtual void RenderUI() const override {};

	int GetPoints() { return m_Points; }
	void PickUpPoints(int amountOfPoints)
	{
		m_Points += amountOfPoints;
		if (m_Subject != nullptr) m_Subject->NotifyObservers(dae::Event{ dae::make_sdbm_hash("PlayerScored") });
	}

private:
	int m_Points{};
	dae::Subject* m_Subject{ nullptr };
};


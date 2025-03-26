#pragma once
#include "Component.h"
#include "Observer.h"
#include <vector>

namespace dae
{
	class Subject : public Component
	{
	public:
		Subject(GameObject* gameObject)
			:Component(gameObject)
		{

		}

		void NotifyObservers(const Event& event)
		{
			for (auto observer : m_observers)
			{
				observer->Notify(event, m_gameObject);
			}
		}

		void AddObserver(Observer* observer)
		{
			m_observers.emplace_back(observer);
		}

		void RemoveObserver(Observer* observer)
		{
			auto it = std::find(std::begin(m_observers), std::end(m_observers), observer);
			m_observers.erase(it);
		}

		virtual void Start() override{}
		virtual void Update() override{}
		virtual void LateUpdate() override{}
		virtual void Render() const override{}
		virtual void RenderUI() const override{}

	private:
		std::vector<Observer*> m_observers{};
	};
}


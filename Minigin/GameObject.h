#pragma once
#include <memory>
#include "Transform.h"
#include "Component.h"
#include <vector>

namespace dae
{
	class Texture2D;
	class GameObject final
	{
	public:
		virtual void Start();
		virtual void Update(float deltaTime);
		virtual void FixedUpdate(float fixedTime);
		virtual void LateUpdate(float deltaTime);
		virtual void Render() const;

		void SetPosition(float x, float y);

		void AddComponent(std::unique_ptr<Component> component);
		void RemoveComponent(Component* component);

		template <typename C>
		C* GetComponent() const
		{
			for (const auto& component : m_components)
			{
				C* castedComponent = dynamic_cast<C*>(component.get());
				if (castedComponent)
				{
					return castedComponent;
				}
			}
			return nullptr;
		}


		template <typename C>
		bool CheckComponent() const
		{
			return GetComponent<C>() != nullptr;
		}

		Transform GetTransform() const;

		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		Transform m_transform{};
		std::vector<std::unique_ptr<Component>> m_components{};
		std::vector<Component*> m_unregisteredComponents{};
	};
}

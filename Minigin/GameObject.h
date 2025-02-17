#pragma once
#include <memory>
#include "Transform.h"
#include <vector>

namespace dae
{
	class Texture2D;
	class Component;
	class GameObject final
	{
	public:
		virtual void Start();
		virtual void Update(float deltaTime);
		virtual void FixedUpdate(float fixedTime);
		virtual void Render() const;

		void SetPosition(float x, float y);

		void AddComponent(std::shared_ptr<Component> component);
		void RemoveComponent(std::shared_ptr<Component> component);

		template <typename C>
		std::shared_ptr<C> GetComponent() const
		{
			for (const std::shared_ptr<Component>& component : m_components)
			{
				std::shared_ptr<C> castedPointer = std::dynamic_pointer_cast<C>(component);
				if (castedPointer)
				{
					return castedPointer;
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
		std::vector<std::shared_ptr<Component>> m_components{};
	};
}

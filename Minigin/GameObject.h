#pragma once
#include <memory>
#include "Transform.h"
#include "Component.h"
#include <vector>

namespace dae
{
	struct Event;
	class Observer;
	class Texture2D;
	class BoxColliderComponent;
	class GameObject final
	{
	public:
		virtual void Start();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void LateUpdate();
		virtual void Render() const;
		virtual void RenderUI() const;
		virtual void OnCollisionEnter(const BoxColliderComponent& other);

		void SetPosition(float x, float y);
		void SetWorldPosition(float x, float y);

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

		void SetParent(GameObject* parent, bool keepWorldPosition);
		GameObject* GetParent() const;
		GameObject* GetChildAt(int index) const;
		int GetChildCount() const;
		bool IsChild(GameObject* object) const;

		void SetLocalPosition(const glm::vec3& pos);
		const glm::vec3& GetWorldPosition();
		void UpdateWorldPosition();
		void SetPositionDirty();

		void Destroy();
		bool ToBeDestroyed() const;

		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		void RemoveChild(GameObject* child);
		void AddChild(GameObject* child);

		Transform m_transform{};
		glm::vec3 m_worldPosition{};
		bool m_positionIsDirty{ false };
		bool m_ToBeDestroyed{ false };

		std::vector<std::unique_ptr<Component>> m_components{};
		std::vector<Component*> m_unregisteredComponents{};

		GameObject* m_parent{};
		std::vector<GameObject*> m_children{};
	};
}

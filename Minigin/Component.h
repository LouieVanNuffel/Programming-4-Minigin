#pragma once
#include <memory>

namespace dae
{
	class GameObject;
	class BoxColliderComponent;
	class Component
	{
	public:
		virtual void Start() = 0;
		virtual void Update() = 0;
		virtual void LateUpdate() = 0;
		virtual void Render() const = 0;
		virtual void RenderUI() const = 0;
		virtual void OnCollisionEnter(const BoxColliderComponent&) {};

		explicit Component(GameObject* gameObject)
			:m_gameObject{ gameObject }
		{}
		virtual ~Component() = default;
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

	protected:
		GameObject* m_gameObject;
	};
}


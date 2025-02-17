#pragma once
#include "Component.h"
#include <memory>

namespace dae
{
	class GameObject;
	class Component
	{
	public:
		virtual void Start() = 0;
		virtual void Update(float deltaTime) = 0;
		virtual void Render() const = 0;

		explicit Component(std::shared_ptr<GameObject> gameObject)
			:m_gameObject{ gameObject }
		{}
		virtual ~Component() = default;
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

	protected:
		std::weak_ptr<GameObject> m_gameObject; // to avoid circular reference
	};
}


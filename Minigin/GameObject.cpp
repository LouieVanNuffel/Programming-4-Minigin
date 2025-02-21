#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::~GameObject()
{
	m_components.clear();
}

void dae::GameObject::Start()
{
	for (auto& component : m_components)
	{
		component->Start();
	}
}

void dae::GameObject::Update(float deltaTime)
{
	for (auto& component : m_components)
	{
		component->Update(deltaTime);
	}
}

void dae::GameObject::FixedUpdate(float){}

void dae::GameObject::LateUpdate(float deltaTime)
{
	for (auto& component : m_components)
	{
		component->LateUpdate(deltaTime);
	}

	for (auto& unregisteredComponent : m_unregisteredComponents)
	{
		auto it = std::remove_if(m_unregisteredComponents.begin(), m_unregisteredComponents.end(),
			[unregisteredComponent](const Component* c)
			{
				return c == unregisteredComponent; // Compare raw pointers
			});

		if (it != m_unregisteredComponents.end())
		{
			m_unregisteredComponents.erase(it, m_unregisteredComponents.end()); // Erase the matching element
		}
	}
}

void dae::GameObject::Render() const
{
	for (auto& component : m_components)
	{
		component->Render();
	}
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}

void dae::GameObject::AddComponent(std::unique_ptr<Component> component)
{
	assert(component.get() != nullptr);
	if (component.get() != nullptr) m_components.push_back(std::move(component));
}

void dae::GameObject::RemoveComponent(Component* component)
{
	auto it = std::remove_if(m_components.begin(), m_components.end(),
		[component](const std::unique_ptr<Component>& c)
		{
			return c.get() == component; // Compare raw pointers
		});

	if (it != m_components.end())
	{
		m_unregisteredComponents.push_back((*it).get());
	}
}

dae::Transform dae::GameObject::GetTransform() const
{
	return m_transform;
}

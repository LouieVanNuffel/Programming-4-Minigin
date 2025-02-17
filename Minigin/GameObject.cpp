#include <string>
#include "GameObject.h"
#include "Component.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::~GameObject()
{
	for (auto& component : m_components)
	{
		component.reset(); // delete for shared pointer
	}
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

void dae::GameObject::AddComponent(std::shared_ptr<Component> component)
{
	m_components.push_back(component);
}

void dae::GameObject::RemoveComponent(std::shared_ptr<Component> component)
{
	auto it = std::remove(m_components.begin(), m_components.end(), component);
	for (auto iter = it; iter != m_components.end(); ++iter)
	{
		(*iter).reset();  // release shared_ptr
	}
	m_components.erase(it, m_components.end());
}

dae::Transform dae::GameObject::GetTransform() const
{
	return m_transform;
}

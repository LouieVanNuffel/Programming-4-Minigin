#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Observer.h"

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

void dae::GameObject::Update()
{
	for (auto& component : m_components)
	{
		component->Update();
	}
}

void dae::GameObject::FixedUpdate(){}

void dae::GameObject::LateUpdate()
{
	for (auto& component : m_components)
	{
		component->LateUpdate();
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

void dae::GameObject::RenderUI() const
{
	for (auto& component : m_components)
	{
		component->RenderUI();
	}
}

void dae::GameObject::SetPosition(float x, float y)
{
	SetLocalPosition(glm::vec3{ x, y , 0.0f });
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

void dae::GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
{
	if (IsChild(parent) || parent == this || m_parent == parent)
		return;
	if (parent == nullptr)
		SetLocalPosition(GetWorldPosition());
	else
	{
		if (keepWorldPosition)
			SetLocalPosition(GetWorldPosition() - parent->GetWorldPosition());
		SetPositionDirty();
	}
	if (m_parent) m_parent->RemoveChild(this);
	m_parent = parent;
	if (m_parent) m_parent->AddChild(this);
}

dae::GameObject* dae::GameObject::GetParent() const
{
	return m_parent;
}

dae::GameObject* dae::GameObject::GetChildAt(int index) const
{
	return m_children[index];
}

int dae::GameObject::GetChildCount() const
{
	return static_cast<int>(m_children.size());
}

bool dae::GameObject::IsChild(GameObject* object) const
{
	for(GameObject* child : m_children)
	{
		if (child == object) return true;
	}

	return false;
}

void dae::GameObject::NotifyObservers(const Event& event)
{
	for (auto observer : m_observers)
	{
		observer->Notify(event, this);
	}
}

void dae::GameObject::RemoveChild(GameObject* child)
{
	m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
}

void dae::GameObject::AddChild(GameObject* child)
{
	m_children.emplace_back(child);
}

void dae::GameObject::SetLocalPosition(const glm::vec3& pos)
{
	m_transform.SetPosition(pos.x, pos.y, pos.z);
	SetPositionDirty();
}

const glm::vec3& dae::GameObject::GetWorldPosition()
{
	if (m_positionIsDirty)
		UpdateWorldPosition();
	return m_worldPosition;
}

void dae::GameObject::UpdateWorldPosition()
{
	if (m_positionIsDirty)
	{
		if (m_parent == nullptr)
			m_worldPosition = m_transform.GetPosition();
		else
			m_worldPosition = m_parent->GetWorldPosition() + m_transform.GetPosition();
	}
	m_positionIsDirty = false;
}

void dae::GameObject::SetPositionDirty()
{
	m_positionIsDirty = true;
	for(GameObject* child : m_children)
	{
		child->SetPositionDirty();
	}
}

void dae::GameObject::AddObserver(Observer* observer)
{
	m_observers.emplace_back(observer);
}

void dae::GameObject::RemoveObserver(Observer* observer)
{
	auto it = std::find(std::begin(m_observers), std::end(m_observers), observer);
	m_observers.erase(it);
}

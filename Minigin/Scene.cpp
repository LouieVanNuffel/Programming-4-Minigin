#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void dae::Scene::Start()
{
	for (auto& object : m_objects)
	{
		object->Start();
	}
}

void Scene::Update()
{
	for(auto& object : m_objects)
	{
		object->Update();
	}
}

void dae::Scene::FixedUpdate()
{
	for (auto& object : m_objects)
	{
		object->FixedUpdate();
	}
}

void dae::Scene::LateUpdate()
{
	for (auto& object : m_objects)
	{
		object->LateUpdate();
	}
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

void dae::Scene::RenderUI() const
{
	for (const auto& object : m_objects)
	{
		object->RenderUI();
	}
}

void dae::Scene::CleanUpDestroyed()
{
	std::vector<int> indicesToBeDestroyed{};
	for (uint32_t index{}; index < m_objects.size(); ++index)
	{
		if (m_objects[index]->ToBeDestroyed())
		{
			m_objects[index]->OnDestroy();
			indicesToBeDestroyed.emplace_back(index);
		}
	}

	for (const int& index : indicesToBeDestroyed)
	{
		auto it = std::find(m_objects.begin(), m_objects.end(), m_objects[index]);
		if (it != m_objects.end())
		{
			m_objects.erase(it);
		}
	}
}

void dae::Scene::OnActivate()
{
	// Go through loop once without rendering
	Start();
	Update();
	FixedUpdate();
	LateUpdate();
}


#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Start()
{
	for (auto& scene : m_scenes)
	{
		scene->Start();
	}
}

void dae::SceneManager::Update()
{
	for(auto& scene : m_scenes)
	{
		scene->Update();
	}
}

void dae::SceneManager::FixedUpdate()
{
	for (auto& scene : m_scenes)
	{
		scene->FixedUpdate();
	}
}

void dae::SceneManager::LateUpdate()
{
	for (auto& scene : m_scenes)
	{
		scene->LateUpdate();
	}
}

void dae::SceneManager::Render()
{
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
}

void dae::SceneManager::RenderUI()
{
	for (const auto& scene : m_scenes)
	{
		scene->RenderUI();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}

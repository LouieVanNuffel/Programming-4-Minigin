#include "SceneManager.h"
#include "Scene.h"
#include <iostream>

void dae::SceneManager::Start()
{
	if (m_scenes.empty()) return;
	m_scenes[m_ActiveSceneIndex]->Start();
}

void dae::SceneManager::Update()
{
	if (m_scenes.empty()) return;
	m_scenes[m_ActiveSceneIndex]->Update();
}

void dae::SceneManager::FixedUpdate()
{
	if (m_scenes.empty()) return;
	m_scenes[m_ActiveSceneIndex]->FixedUpdate();
}

void dae::SceneManager::LateUpdate()
{
	if (m_scenes.empty()) return;
	m_scenes[m_ActiveSceneIndex]->LateUpdate();
}

void dae::SceneManager::Render()
{
	if (m_scenes.empty()) return;
	m_scenes[m_ActiveSceneIndex]->Render();
}

void dae::SceneManager::RenderUI()
{
	if (m_scenes.empty()) return;
	m_scenes[m_ActiveSceneIndex]->RenderUI();
}

void dae::SceneManager::CleanUpDestroyed()
{
	if (m_scenes.empty()) return;
	m_scenes[m_ActiveSceneIndex]->CleanUpDestroyed();
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}

void dae::SceneManager::SetActiveScene(uint32_t sceneIndex)
{
	if (sceneIndex >= 0 && sceneIndex < m_scenes.size()) m_ActiveSceneIndex = sceneIndex;
	else std::cerr << "scene index given is invalid! scene index was: " << sceneIndex << std::endl;
}

void dae::SceneManager::SetActiveScene(const std::string& name)
{
	auto it = std::find_if(m_scenes.begin(), m_scenes.end(), [name](const auto& scene) 
		{
			return scene->Name() == name;
		});

	if (it == m_scenes.end())
	{
		std::cerr << "scene name given is invalid! scene name was: " << name << std::endl;
		return;
	}
	
	uint32_t index = static_cast<uint32_t>(std::distance(m_scenes.begin(), it));
	m_ActiveSceneIndex = index;
}

uint32_t dae::SceneManager::ActiveSceneIndex() const
{
	return m_ActiveSceneIndex;
}

void dae::SceneManager::AddGameObjectToScene(std::shared_ptr<GameObject> gameObject, uint32_t sceneIndex)
{
	if (sceneIndex >= 0 && sceneIndex < m_scenes.size())
	{
		m_scenes[sceneIndex]->Add(std::move(gameObject));
	}
	else std::cerr << "scene index given is invalid! scene index was: " << sceneIndex << std::endl;
}

dae::Scene& dae::SceneManager::ActiveScene()
{
	return *m_scenes[m_ActiveSceneIndex];
}

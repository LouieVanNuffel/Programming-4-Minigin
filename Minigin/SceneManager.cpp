#include "SceneManager.h"
#include "Scene.h"
#include <iostream>
#include "EngineTime.h"

void dae::SceneManager::Start()
{
	if (m_scenes.empty()) return;
	m_scenes[m_ActiveSceneIndex]->Start();
}

void dae::SceneManager::Update()
{
	if (!m_CanSwitch)
	{
		m_SwitchCooldownTimer -= dae::Time::GetInstance().GetDeltaTime();
		if (m_SwitchCooldownTimer <= 0.0f) m_CanSwitch = true;
	}

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
	if (!m_CanSwitch) return;
	m_CanSwitch = false;
	m_SwitchCooldownTimer = m_SwitchCooldown;

	if (sceneIndex >= 0 && sceneIndex < m_scenes.size())
	{
		m_ActiveSceneIndex = sceneIndex;
		m_scenes[m_ActiveSceneIndex]->OnActivate();
		m_CanSwitch = false;
	}
	else std::cerr << "scene index given is invalid! scene index was: " << sceneIndex << std::endl;
}

void dae::SceneManager::SetActiveScene(const std::string& name)
{
	if (!m_CanSwitch) return;
	m_CanSwitch = false;

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
	m_scenes[m_ActiveSceneIndex]->OnActivate();
	m_CanSwitch = true;
}

void dae::SceneManager::LoadNextScene()
{
	if (!m_CanSwitch) return;

	uint32_t nextIndex = m_ActiveSceneIndex + 1;
	nextIndex %= m_scenes.size(); // keep within bounds (loop around)
	SetActiveScene(nextIndex);
	std::cout << "loaded scene " << nextIndex << std::endl;
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

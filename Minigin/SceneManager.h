#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"

namespace dae
{
	class Scene;
	class GameObject;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);
		void SetActiveScene(uint32_t sceneIndex);
		void SetActiveScene(const std::string& name);
		void LoadNextScene();
		uint32_t ActiveSceneIndex() const;
		void AddGameObjectToScene(std::shared_ptr<GameObject> gameObject, uint32_t sceneIndex);
		Scene& ActiveScene();

		void Start();
		void Update();
		void FixedUpdate();
		void LateUpdate();
		void Render();
		void RenderUI();
		void CleanUpDestroyed();

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_scenes;
		uint32_t m_ActiveSceneIndex{ 0 };

		bool m_CanSwitch{ true };
		float m_SwitchCooldown{ 1.0f };
		float m_SwitchCooldownTimer{};
	};
}

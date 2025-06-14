#pragma once
#include "Component.h"
#include "Observer.h"
#include "Event.h"
#include "LevelState.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "TileMovementComponent.h"
#include "Scene.h"

class RespawnComponent final : public dae::Component, public dae::Observer
{
public:
	RespawnComponent(dae::GameObject* gameObject)
		:dae::Component(gameObject)
	{

	}

	virtual void Start() override 
	{
		m_TileMovementComponent = m_gameObject->GetComponent<TileMovementComponent>();
	};

	virtual void Update() override {};
	virtual void LateUpdate() override {};
	virtual void Render() const override {};
	virtual void RenderUI() const override {};

private:
	TileMovementComponent* m_TileMovementComponent{ nullptr };

	virtual void Notify(const dae::Event& event, const dae::GameObject*) override
	{
		if (event.id == dae::make_sdbm_hash("TookDamage"))
		{
			dae::SceneManager::GetInstance().ActiveScene().levelState->Respawn(); // Tells the other objects to respawn (snobees)
			glm::vec3 respawnPosition = dae::SceneManager::GetInstance().ActiveScene().levelState->GetPlayerSpawnPosition();
			m_gameObject->SetWorldPosition(respawnPosition.x, respawnPosition.y);
			if (m_TileMovementComponent != nullptr) m_TileMovementComponent->Reset();
		}
	}

};

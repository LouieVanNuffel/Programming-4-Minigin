#pragma once
#include "Component.h"
#include "Observer.h"
#include "Event.h"
#include "LevelState.h"
#include "GameObject.h"

class RespawnComponent final : public dae::Component, public dae::Observer
{
public:
	RespawnComponent(dae::GameObject* gameObject)
		:dae::Component(gameObject)
	{

	}

	virtual void Start() override {};
	virtual void Update() override {};
	virtual void LateUpdate() override {};
	virtual void Render() const override {};
	virtual void RenderUI() const override {};

private:
	virtual void Notify(const dae::Event& event, const dae::GameObject*) override
	{
		if (event.id == dae::make_sdbm_hash("TookDamage"))
		{
			LevelState::GetInstance().Respawn(); // Tells the other objects to respawn (snobees)
			glm::vec3 respawnPosition = LevelState::GetInstance().GetPlayerSpawnPosition();
			m_gameObject->SetWorldPosition(respawnPosition.x, respawnPosition.y);
		}
	}

};

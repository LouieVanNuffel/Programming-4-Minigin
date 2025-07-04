#pragma once
#include "Component.h"
#include "LevelState.h"
#include "Event.h"
#include "SceneManager.h"
#include "Scene.h"

class PlayerTrackingComponent final : public dae::Component, public dae::Observer
{
public:
	PlayerTrackingComponent(dae::GameObject* gameObject)
		:dae::Component(gameObject)
	{
		
	}

	virtual void Start() 
	{
		dae::SceneManager::GetInstance().ActiveScene().levelState->AddPlayerObject(m_gameObject);
	};
	virtual void Update() {};
	virtual void LateUpdate() {};
	virtual void Render() const {};
	virtual void RenderUI() const {};

	virtual void Notify(const dae::Event& event, const dae::GameObject*) override
	{
		if (event.id == dae::make_sdbm_hash("Died"))
		{
			dae::SceneManager::GetInstance().ActiveScene().levelState->RemovePlayerObject(m_gameObject);
		}
	};
};

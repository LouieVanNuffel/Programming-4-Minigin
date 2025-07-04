#pragma once
#include "Component.h"
#include "LevelState.h"
#include "EngineTime.h"
#include "SceneManager.h"
#include "Scene.h"

class LevelTimerComponent final : public dae::Component
{
public:
	//Constructor
	LevelTimerComponent(dae::GameObject* gameObject)
		:dae::Component(gameObject)
	{

	};

	virtual void Start() {};
	virtual void Update() 
	{
		dae::SceneManager::GetInstance().ActiveScene().levelState->AddTime(dae::Time::GetInstance().GetDeltaTime());
	};
	virtual void LateUpdate() {};
	virtual void Render() const {};
	virtual void RenderUI() const {};
};
#pragma once
#include "Component.h"
#include "LevelState.h"
#include "EngineTime.h"

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
		LevelState::GetInstance().AddTime(dae::Time::GetInstance().GetDeltaTime());
	};
	virtual void LateUpdate() {};
	virtual void Render() const {};
	virtual void RenderUI() const {};
};
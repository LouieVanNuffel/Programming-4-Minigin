#pragma once
#include "Component.h"
#include "LevelState.h"

class PlayerTrackingComponent final : public dae::Component
{
public:
	PlayerTrackingComponent(dae::GameObject* gameObject)
		:dae::Component(gameObject)
	{
		LevelState::GetInstance().AddPlayerObject(gameObject);
	}

	virtual void Start() {};
	virtual void Update() {};
	virtual void LateUpdate() {};
	virtual void Render() const {};
	virtual void RenderUI() const {};
	virtual void OnDestroy()
	{
		LevelState::GetInstance().RemovePlayerObject(m_gameObject);
	};
};

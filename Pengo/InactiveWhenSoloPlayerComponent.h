#pragma once
#include "Component.h"
#include "GameState.h"
#include "GameObject.h"

class InactiveWhenSoloPlayerComponent final : public dae::Component
{
public:
	InactiveWhenSoloPlayerComponent(dae::GameObject* gameObject)
		:dae::Component(gameObject)
	{

	}

	virtual void Start() override 
	{
		if (GameState::GetInstance().PlayerCount() == 1)
		{
			m_gameObject->SetActive(false);
		}
	};

	virtual void Update() override {};
	virtual void LateUpdate() override {};
	virtual void Render() const override {};
	virtual void RenderUI() const override {};
};

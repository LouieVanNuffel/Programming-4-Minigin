#pragma once
#include "Animator.h"
#include "AnimationState.h"
#include "ResourceManager.h"

class IdleState final : public AnimationState
{
public:
	IdleState(Animator* animator)
		:AnimationState(animator), m_Texture{}
	{
		m_StateToTransitionTo = AnimationStates::idle;
		m_Texture = dae::ResourceManager::GetInstance().LoadTexture("PengoRed.png");
	}

	virtual void Update() override
	{

	}

	virtual void OnEnter() override
	{
		m_pAnimator->SetTexture(m_Texture);
	}

	virtual void OnExit() override
	{

	}

private:
	std::shared_ptr<dae::Texture2D> m_Texture;
};

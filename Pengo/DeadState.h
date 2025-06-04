#pragma once
#include "Animator.h"
#include "AnimationState.h"
#include "ResourceManager.h"

class DeadState final : public AnimationState
{
public:
	DeadState(Animator* animator)
		:AnimationState(animator), m_Texture{}
	{
		m_StateToTransitionTo = AnimationStates::dead;
		m_Texture = dae::ResourceManager::GetInstance().LoadTexture("PengoDead.png");
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

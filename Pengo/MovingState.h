#pragma once
#include "Animator.h"
#include "AnimationState.h"
#include "ResourceManager.h"
#include < iostream>

class MovingState final : public AnimationState
{
public:
	MovingState(Animator* animator)
		:AnimationState(animator), m_Texture{}
	{
		m_StateToTransitionTo = AnimationStates::moving;
		m_Texture = dae::ResourceManager::GetInstance().LoadTexture("PengoUp.png");
	}

	virtual void Update() override
	{

	}

	virtual void OnEnter() override
	{
		m_pAnimator->SetTexture(m_Texture);
		std::cout << "g" << std::endl;
	}

	virtual void OnExit() override
	{

	}

private:
	std::shared_ptr<dae::Texture2D> m_Texture;
};

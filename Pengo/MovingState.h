#pragma once
#include "Animator.h"
#include "AnimationState.h"
#include "ResourceManager.h"
#include < iostream>

class MovingState final : public AnimationState
{
public:
	MovingState(Animator* animator)
		:AnimationState(animator)
	{
		m_StateToTransitionTo = AnimationStates::moving;
	}

	virtual void Update() override
	{

	}

	virtual void OnEnter() override
	{
		m_pAnimator->AddSourceRectPosition(m_SourceRectOffsetX, m_SourceRectOffsetY);
	}

	virtual void OnExit() override
	{
		m_pAnimator->AddSourceRectPosition(-m_SourceRectOffsetX, -m_SourceRectOffsetY);
	}

private:
	int m_SourceRectOffsetX{ 4 * 16 };
	int m_SourceRectOffsetY{ 0 };
};

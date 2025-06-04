#pragma once
#include "Animator.h"
#include "AnimationState.h"
#include "ResourceManager.h"

class DeadState final : public AnimationState
{
public:
	DeadState(Animator* animator)
		:AnimationState(animator)
	{
		m_StateToTransitionTo = AnimationStates::dead;
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
	int m_SourceRectOffsetX{ 5 * 16 };
	int m_SourceRectOffsetY{ 3 * 16 };
};

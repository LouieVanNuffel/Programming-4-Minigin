#pragma once
#include "Animator.h"
#include "AnimationState.h"
#include "ResourceManager.h"

class IdleState final : public AnimationState
{
public:
	IdleState(Animator* animator)
		:AnimationState(animator)
	{
		m_StateToTransitionTo = AnimationStates::idle;
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
	int m_SourceRectOffsetX{ 0 };
	int m_SourceRectOffsetY{ 0 };
};

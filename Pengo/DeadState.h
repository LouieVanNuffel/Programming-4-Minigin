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
		m_pAnimationSequence = new AnimationSequence(16, 0, 8, 2, true);
	}

	virtual void Update() override
	{
		m_pAnimationSequence->Update();
		m_pAnimator->AddSourceRectPositionToStartPosition(m_SourceRectOffsetX + m_pAnimationSequence->GetOffsetX(), 
														  m_SourceRectOffsetY + m_pAnimationSequence->GetOffsetY());
	}

	virtual void OnEnter() override
	{
		
	}

	virtual void OnExit() override
	{

	}

	virtual void Notify(const dae::Event&, const dae::GameObject*) override
	{
		// Does nothing because once dead, no state transitions should happen anymore
	}

private:
	int m_SourceRectOffsetX{ 0 };
	int m_SourceRectOffsetY{ 2 * 16 };
};

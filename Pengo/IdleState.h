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
		m_pAnimationSequence = new AnimationSequence(16, 0, 8, 1, false);
	}

	virtual void Update() override
	{
		UpdateOffset();

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

private:
	void UpdateOffset()
	{
		// only works because order of direction enum class matches up with order of sourceRectOffsetsX
		m_SourceRectOffsetX = m_SourceRectOffsetsX[static_cast<int>(m_pAnimator->GetDirection())];
	}

	int m_SourceRectOffsetX{ 0 };
	int m_SourceRectOffsetY{ 0 };
	std::vector<int> m_SourceRectOffsetsX{ 4 * 16, 0 * 16, 2 * 16, 6 * 16 };
};

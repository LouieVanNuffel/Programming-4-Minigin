#include "AnimationState.h"
#include "Animator.h"
#include "Event.h"

void AnimationState::Update()
{
	UpdateOffset();

	m_pAnimationSequence->Update();
	m_pAnimator->AddSourceRectPositionToStartPosition(m_SourceRectOffsetX + m_pAnimationSequence->GetOffsetX(),
		m_SourceRectOffsetY + m_pAnimationSequence->GetOffsetY());
}

void AnimationState::Notify(const dae::Event& event, const dae::GameObject*)
{
	if (m_StateToTransitionTo == AnimationStates::dead) return;

	if (event.id == dae::make_sdbm_hash("Moved"))
	{
		m_StateToTransitionTo = AnimationStates::moving;
	}

	if (event.id == dae::make_sdbm_hash("DidNotMove"))
	{
		m_StateToTransitionTo = AnimationStates::idle;
	}

	if (event.id == dae::make_sdbm_hash("Died"))
	{
		m_StateToTransitionTo = AnimationStates::dead;
	}
}

void AnimationState::UpdateOffset()
{
	// only works because order of direction enum class matches up with order of sourceRectOffsetsX
	m_SourceRectOffsetX = m_SourceRectOffsetsX[static_cast<int>(m_pAnimator->GetDirection())];
}

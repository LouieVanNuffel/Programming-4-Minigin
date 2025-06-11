#include "AnimationState.h"
#include "Animator.h"

AnimationState::AnimationState(Animator* animator, const AnimationStateData& data)
    : m_pAnimator(animator),
    m_pAnimationSequence{ std::make_unique<AnimationSequence>(data.animationSequenceData) },
    m_SourceRectOffsetsX{ data.sourceRectOffsetsX },
    m_SourceRectOffsetY{ data.sourceRectOffsetY },
    m_StateType{ data.thisAnimationState }
{

}

void AnimationState::Update() 
{
    m_pAnimationSequence->Update();
    UpdateOffsetAndRenderRect();
}

void AnimationState::UpdateOffsetAndRenderRect() 
{
    int directionIndex = static_cast<int>(m_pAnimator->GetDirection());
    int sourceRectOffsetX = m_SourceRectOffsetsX[directionIndex];

    m_pAnimator->AddSourceRectPositionToStartPosition(sourceRectOffsetX + m_pAnimationSequence->GetOffsetX(),
                                                      m_SourceRectOffsetY + m_pAnimationSequence->GetOffsetY());
}

AnimationStates AnimationState::GetStateType() const 
{
    return m_StateType;
}

#include "StunnedState.h"
#include "Animator.h"
#include "EngineTime.h"

StunnedState::StunnedState(Animator* animator, const AnimationStateData& data, float stunDuration)
    : AnimationState(animator, data), m_StunTimeRemaining(stunDuration), m_TotalStunTime(stunDuration) 
{

}

void StunnedState::OnEnter() 
{
    m_StunTimeRemaining = m_TotalStunTime;
}

void StunnedState::Update() 
{
    AnimationState::Update();

    m_StunTimeRemaining -= dae::Time::GetInstance().GetDeltaTime();
    if (m_StunTimeRemaining <= 0.f) {
        m_pAnimator->ChangeState(AnimationStates::idle); // Or whatever state makes sense
    }
}

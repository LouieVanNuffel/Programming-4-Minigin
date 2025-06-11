#pragma once
#include "AnimationState.h"

class IdleState final : public AnimationState {
public:
    IdleState(Animator* animator, const AnimationStateData& data)
        : AnimationState(animator, data)
    {

    }

    void Notify(const dae::Event& event, const dae::GameObject*) override 
    {
        if (event.id == dae::make_sdbm_hash("Moved")) 
        {
            m_pAnimator->ChangeState(AnimationStates::moving);
        }

        if (event.id == dae::make_sdbm_hash("Died")) 
        {
            m_pAnimator->ChangeState(AnimationStates::dead);
        }

        if (event.id == dae::make_sdbm_hash("Stunned")) 
        {
            m_pAnimator->ChangeState(AnimationStates::stunned);
        }
    }
};

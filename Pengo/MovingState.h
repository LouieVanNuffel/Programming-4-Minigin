#pragma once
#include "AnimationState.h"

class MovingState final : public AnimationState {
public:
    MovingState(Animator* animator, const AnimationStateData& data)
        : AnimationState(animator, data) {}

    void Notify(const dae::Event& event, const dae::GameObject*) override {
        if (event.id == dae::make_sdbm_hash("DidNotMove")) {
            m_pAnimator->ChangeState(AnimationStates::idle);
        }
        if (event.id == dae::make_sdbm_hash("Died")) {
            m_pAnimator->ChangeState(AnimationStates::dead);
        }
    }
};

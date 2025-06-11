#pragma once
#include "AnimationState.h"

class DeadState final : public AnimationState {
public:
    DeadState(Animator* animator, const AnimationStateData& data)
        : AnimationState(animator, data) {}

    void OnEnter() override {
        
    }

    void Notify(const dae::Event&, const dae::GameObject*) override {
        // Ignore all events when dead
    }
};

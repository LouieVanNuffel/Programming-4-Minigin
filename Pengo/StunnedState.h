#pragma once
#include "AnimationState.h"

class StunnedState final : public AnimationState 
{
public:
    StunnedState(Animator* animator, const AnimationStateData& data, float stunDuration);

    void Update() override;
    void OnEnter() override;

private:
    float m_StunTimeRemaining{};
    float m_TotalStunTime{};
};

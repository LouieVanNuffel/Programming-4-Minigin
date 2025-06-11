#pragma once
#include "Observer.h"
#include "AnimationSequence.h"
#include <memory>
#include <vector>

enum class AnimationStates
{
	idle, dead, moving, stunned
};

struct AnimationStateData
{
	AnimationStates thisAnimationState;
	AnimationSequenceData animationSequenceData;
	int sourceRectOffsetX;
	int sourceRectOffsetY;
	std::vector<int> sourceRectOffsetsX; // Must match order of Direction enum class in Animator (up, down, left, right)
};

class Animator;
class AnimationState : public dae::Observer {
public:
    AnimationState(Animator* animator, const AnimationStateData& data);
    virtual ~AnimationState() = default;

    virtual void Update();
    virtual void OnEnter() {}
    virtual void OnExit() {}

    virtual void Notify(const dae::Event&, const dae::GameObject*) override {}

    AnimationStates GetStateType() const;

protected:
    Animator* m_pAnimator;

    std::unique_ptr<AnimationSequence> m_pAnimationSequence;

    int m_SourceRectOffsetY;
    std::vector<int> m_SourceRectOffsetsX;

    AnimationStates m_StateType;

    void UpdateOffsetAndRenderRect();
};

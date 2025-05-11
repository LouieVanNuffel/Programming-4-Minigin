#pragma once
#include "Observer.h"
#include "Event.h"

enum class AnimationStates
{
	idle, dead, left, right, up
};

class Animator;
class AnimationState : public dae::Observer
{
public:
	AnimationState(Animator* animator)
		: m_pAnimator{ animator }, m_StateToTransitionTo{}
	{

	}

	virtual AnimationState* GetNewState() { return nullptr; }

	virtual void Update() {};
	virtual void OnEnter() {};
	virtual void OnExit() {};
	virtual void Notify(const dae::Event&, const dae::GameObject*) override {};

protected:
	Animator* m_pAnimator;
	AnimationStates m_StateToTransitionTo;
};

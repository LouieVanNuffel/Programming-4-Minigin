#pragma once
#include "Observer.h"
#include "Event.h"

using namespace dae;

enum class AnimationStates
{
	idle, dead
};

class Animator;
class AnimationState : public Observer
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
	virtual void Notify(const Event&, const GameObject*) override {};

protected:
	Animator* m_pAnimator;
	AnimationStates m_StateToTransitionTo;
};

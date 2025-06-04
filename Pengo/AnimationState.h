#pragma once
#include "Observer.h"
#include "Event.h"

enum class AnimationStates
{
	idle, dead
};

class Animator;
class AnimationState : public dae::Observer
{
public:
	AnimationState(Animator* animator)
		: m_pAnimator{ animator }, m_StateToTransitionTo{}
	{

	}

	const AnimationStates& GetNewStateToTransitionTo()
	{
		return m_StateToTransitionTo;
	}

	virtual void Update() {};
	virtual void OnEnter() {};
	virtual void OnExit() {};

	virtual void Notify(const dae::Event& event, const dae::GameObject*) override
	{
		if (event.id == dae::make_sdbm_hash("PlayerDied"))
		{
			m_StateToTransitionTo = AnimationStates::dead;
		}
	}

protected:
	Animator* m_pAnimator;
	AnimationStates m_StateToTransitionTo;
};

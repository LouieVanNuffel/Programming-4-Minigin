#pragma once
#include "Observer.h"
#include "Event.h"
#include "AnimationSequence.h"

enum class AnimationStates
{
	idle, dead, moving
};

class Animator;
class AnimationState : public dae::Observer
{
public:
	AnimationState(Animator* animator)
		: m_pAnimator{ animator }
	{

	}

	~AnimationState()
	{
		if (m_pAnimationSequence != nullptr)
		{
			delete m_pAnimationSequence;
			m_pAnimationSequence = nullptr;
		}
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
		if (event.id == dae::make_sdbm_hash("PlayerMoved"))
		{
			m_StateToTransitionTo = AnimationStates::moving;
		}

		if (event.id == dae::make_sdbm_hash("PlayerDidNotMove"))
		{
			m_StateToTransitionTo = AnimationStates::idle;
		}

		if (event.id == dae::make_sdbm_hash("PlayerDied"))
		{
			m_StateToTransitionTo = AnimationStates::dead;
		}
	}

protected:
	Animator* m_pAnimator;
	AnimationStates m_StateToTransitionTo{};
	AnimationSequence* m_pAnimationSequence{ nullptr };
};

#pragma once
#include "Animator.h"
#include "AnimationState.h"
#include "DeadState.h"
#include "ResourceManager.h"

class IdleState final : public AnimationState
{
public:
	IdleState(Animator* animator)
		:AnimationState(animator), m_Texture{}
	{
		m_StateToTransitionTo = AnimationStates::idle;
		m_Texture = ResourceManager::GetInstance().LoadTexture("PengoRed.png");
	}

	virtual AnimationState* GetNewState() override
	{
		switch (m_StateToTransitionTo)
		{
		case AnimationStates::dead:
			return new DeadState{ m_pAnimator };
			break;
		default:
			return nullptr;
			break;
		}
	}

	virtual void Update() override
	{

	}

	virtual void OnEnter() override
	{
		m_pAnimator->SetTexture(m_Texture);
	}

	virtual void OnExit() override
	{

	}

	virtual void Notify(const Event& event, const GameObject*) override
	{
		if (event.id == make_sdbm_hash("PlayerDied"))
		{
			m_StateToTransitionTo = AnimationStates::dead;
		}
	}

private:
	std::shared_ptr<Texture2D> m_Texture;
};

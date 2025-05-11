#pragma once
#include "Animator.h"
#include "AnimationState.h"
#include "ResourceManager.h"

class LeftState final : public AnimationState
{
public:
	LeftState(Animator* animator)
		:AnimationState(animator), m_Texture{}
	{
		m_StateToTransitionTo = AnimationStates::left;
		m_Texture = ResourceManager::GetInstance().LoadTexture("PengoLeft.png");
	}

	virtual AnimationState* GetNewState() override;

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

		if (event.id == make_sdbm_hash("PlayerRight"))
		{
			m_StateToTransitionTo = AnimationStates::right;
		}

		if (event.id == make_sdbm_hash("PlayerUp"))
		{
			m_StateToTransitionTo = AnimationStates::up;
		}

		if (event.id == make_sdbm_hash("PlayerDown"))
		{
			m_StateToTransitionTo = AnimationStates::idle;
		}
	}

private:
	std::shared_ptr<Texture2D> m_Texture;
};

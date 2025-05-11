#pragma once
#include "Animator.h"
#include "AnimationState.h"
#include "ResourceManager.h"

class RightState final : public AnimationState
{
public:
	RightState(Animator* animator)
		:AnimationState(animator), m_Texture{}
	{
		m_StateToTransitionTo = AnimationStates::right;
		m_Texture = dae::ResourceManager::GetInstance().LoadTexture("PengoRight.png");
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

	virtual void Notify(const dae::Event& event, const dae::GameObject*) override
	{
		if (event.id == dae::make_sdbm_hash("PlayerDied"))
		{
			m_StateToTransitionTo = AnimationStates::dead;
		}

		if (event.id == dae::make_sdbm_hash("PlayerLeft"))
		{
			m_StateToTransitionTo = AnimationStates::left;
		}

		if (event.id == dae::make_sdbm_hash("PlayerUp"))
		{
			m_StateToTransitionTo = AnimationStates::up;
		}

		if (event.id == dae::make_sdbm_hash("PlayerDown"))
		{
			m_StateToTransitionTo = AnimationStates::idle;
		}
	}

private:
	std::shared_ptr<dae::Texture2D> m_Texture;
};

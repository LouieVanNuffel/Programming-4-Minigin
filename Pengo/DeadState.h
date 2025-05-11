#pragma once
#include "Animator.h"
#include "AnimationState.h"
#include "ResourceManager.h"

class DeadState final : public AnimationState
{
public:
	DeadState(Animator* animator)
		:AnimationState(animator), m_Texture{}
	{
		m_StateToTransitionTo = AnimationStates::dead;
		m_Texture = ResourceManager::GetInstance().LoadTexture("PengoDead.png");
	}

	virtual AnimationState* GetNewState() override
	{
		/*switch (m_StateToTransitionTo)
		{
		default:
			return nullptr;
			break;
		}*/
		return nullptr;
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

	virtual void Notify(const Event&, const GameObject*) override
	{
		
	}

private:
	std::shared_ptr<Texture2D> m_Texture;
};

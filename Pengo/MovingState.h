#pragma once
#include "Animator.h"
#include "AnimationState.h"
#include "ResourceManager.h"
#include < iostream>

class MovingState final : public AnimationState
{
public:
	MovingState(Animator* animator)
		:AnimationState(animator)
	{
		m_StateToTransitionTo = AnimationStates::moving;
		m_pAnimationSequence = new AnimationSequence(16, 0, 8, 2, true);
	}

	virtual void Update() override
	{
		UpdateOffset();

		m_pAnimationSequence->Update();
		m_pAnimator->AddSourceRectPositionToStartPosition(m_SourceRectOffsetX + m_pAnimationSequence->GetOffsetX(),
			m_SourceRectOffsetY + m_pAnimationSequence->GetOffsetY());
	}

	virtual void OnEnter() override
	{
		
	}

	virtual void OnExit() override
	{

	}

	virtual void Notify(const dae::Event& event, const dae::GameObject* gameObject) override
	{
		AnimationState::Notify(event, gameObject);

		if (event.id == dae::make_sdbm_hash("PlayerMovedLeft"))
		{
			m_Direction = Direction::left;
		}

		if (event.id == dae::make_sdbm_hash("PlayerMovedRight"))
		{
			m_Direction = Direction::right;
		}

		if (event.id == dae::make_sdbm_hash("PlayerMovedUp"))
		{
			m_Direction = Direction::up;
		}

		if (event.id == dae::make_sdbm_hash("PlayerMovedDown"))
		{
			m_Direction = Direction::down;
		}
	}

private:
	void UpdateOffset()
	{
		// only works because order of direction enum class matches up with order of sourceRectOffsetsX
		m_SourceRectOffsetX = m_SourceRectOffsetsX[static_cast<int>(m_Direction)];
	}

	enum class Direction
	{
		up, down, left, right
	};

	Direction m_Direction{ Direction::up };
	int m_SourceRectOffsetX{ 4 * 16 };
	int m_SourceRectOffsetY{ 0 };
	std::vector<int> m_SourceRectOffsetsX{ 4 * 16, 0 * 16, 2 * 16, 6 * 16 };
};

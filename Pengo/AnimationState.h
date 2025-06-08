#pragma once
#include "Observer.h"
#include "AnimationSequence.h"
#include <vector>
#include <memory>

enum class AnimationStates
{
	idle, dead, moving
};

struct AnimationStateData
{
	AnimationStates thisAnimationState;
	AnimationSequenceData animationSequenceData;
	int sourceRectOffsetX;
	int sourceRectOffsetY;
	std::vector<int> sourceRectOffsetsX; // Must match order of Direction enum class in Animator (up, down, left, right)
};

namespace dae
{
	struct Event;
}

class Animator;
class AnimationState : public dae::Observer
{
public:
	AnimationState(Animator* animator, const AnimationStateData& data)
		: m_pAnimator{ animator }, m_StateToTransitionTo{ data.thisAnimationState }, m_SourceRectOffsetX{ data.sourceRectOffsetX },
		m_SourceRectOffsetY{ data.sourceRectOffsetY }, m_SourceRectOffsetsX{ data.sourceRectOffsetsX }
	{
		m_pAnimationSequence = std::make_unique<AnimationSequence>(data.animationSequenceData);
	}

	const AnimationStates& GetNewStateToTransitionTo()
	{
		return m_StateToTransitionTo;
	}

	virtual void Update();
	virtual void OnEnter() {};
	virtual void OnExit() {};

	virtual void Notify(const dae::Event& event, const dae::GameObject*) override;

protected:
	void UpdateOffset();

	Animator* m_pAnimator;
	AnimationStates m_StateToTransitionTo;
	std::unique_ptr<AnimationSequence> m_pAnimationSequence{ nullptr };

	int m_SourceRectOffsetX;
	int m_SourceRectOffsetY;
	std::vector<int> m_SourceRectOffsetsX;
};

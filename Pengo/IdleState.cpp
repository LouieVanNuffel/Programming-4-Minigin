#include "IdleState.h"
#include "DeadState.h"
#include "LeftState.h"
#include "RightState.h"
#include "UpState.h"

AnimationState* IdleState::GetNewState()
{
	switch (m_StateToTransitionTo)
	{
	case AnimationStates::dead:
		return new DeadState{ m_pAnimator };
		break;
	case AnimationStates::left:
		return new LeftState{ m_pAnimator };
		break;
	case AnimationStates::right:
		return new RightState{ m_pAnimator };
		break;
	case AnimationStates::up:
		return new UpState{ m_pAnimator };
		break;
	default:
		return nullptr;
		break;
	}
}

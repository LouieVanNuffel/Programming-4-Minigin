#include "RightState.h"
#include "DeadState.h"
#include "LeftState.h"
#include "UpState.h"
#include "IdleState.h"

AnimationState* RightState::GetNewState()
{
	switch (m_StateToTransitionTo)
	{
	case AnimationStates::dead:
		return new DeadState{ m_pAnimator };
		break;
	case AnimationStates::left:
		return new LeftState{ m_pAnimator };
		break;
	case AnimationStates::up:
		return new UpState{ m_pAnimator };
		break;
	case AnimationStates::idle:
		return new IdleState{ m_pAnimator };
		break;
	default:
		return nullptr;
		break;
	}
}

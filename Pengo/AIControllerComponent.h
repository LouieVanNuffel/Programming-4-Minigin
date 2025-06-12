#pragma once
#include "ControllerComponent.h"

class AIControllerComponent final : public dae::ControllerComponent
{
public:
	AIControllerComponent(dae::GameObject* gameObject)
		:dae::ControllerComponent(gameObject)
	{

	}

private:
	// Doesn't check for input, on AI just call execute action directly
	virtual bool ActionHappened(uint32_t) override 
	{
		return false;
	};
};


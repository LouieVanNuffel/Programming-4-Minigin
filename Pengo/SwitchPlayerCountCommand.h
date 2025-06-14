#pragma once
#include "GameObjectCommand.h"
#include "GameState.h"

class SwitchPlayerCountCommand : public dae::Command
{
public:
	SwitchPlayerCountCommand()
	{
	}

	virtual void Execute() override
	{
		GameState::GetInstance().SwitchPlayerCount();
	}
};


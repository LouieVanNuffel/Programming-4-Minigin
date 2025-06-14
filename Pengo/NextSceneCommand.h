#pragma once
#include "GameObjectCommand.h"
#include "SceneManager.h"

class NextSceneCommand : public dae::Command
{
public:
	NextSceneCommand()
	{
	}

	virtual void Execute() override
	{
		dae::SceneManager::GetInstance().LoadNextScene();
	}
};


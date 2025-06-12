#pragma once
#include "ControllerComponent.h"
#include "InputManager.h"

class KeyboardControllerComponent final : public dae::ControllerComponent
{
public:
	KeyboardControllerComponent(dae::GameObject* gameObject)
		:ControllerComponent(gameObject)
	{

	}

private:
	virtual bool ActionHappened(dae::Action action) override
	{
		auto& inputManager = dae::InputManager::GetInstance();

		switch (action)
		{
		case dae::Action::up:
			return inputManager.IsKeyDown(SDL_SCANCODE_W);
			break;
		case dae::Action::down:
			return inputManager.IsKeyDown(SDL_SCANCODE_S);
			break;
		case dae::Action::left:
			return inputManager.IsKeyDown(SDL_SCANCODE_A);
			break;
		case dae::Action::right:
			return inputManager.IsKeyDown(SDL_SCANCODE_D);
			break;
		case dae::Action::push:
			return inputManager.IsKeyDown(SDL_SCANCODE_SPACE);
			break;
		}

		return false;
	}
};


#pragma once
#include "ControllerComponent.h"
#include "InputManager.h"
#include "Actions.h"

class KeyboardControllerComponent final : public dae::ControllerComponent
{
public:
	KeyboardControllerComponent(dae::GameObject* gameObject)
		:ControllerComponent(gameObject)
	{

	}

private:
	virtual bool ActionHappened(uint32_t action) override
	{
		auto& inputManager = dae::InputManager::GetInstance();
		if (&inputManager == nullptr) return false;
		Action actionEnum = static_cast<Action>(action);

		switch (actionEnum)
		{
		case Action::up:
			return inputManager.IsKeyDown(SDL_SCANCODE_W);
			break;
		case Action::down:
			return inputManager.IsKeyDown(SDL_SCANCODE_S);
			break;
		case Action::left:
			return inputManager.IsKeyDown(SDL_SCANCODE_A);
			break;
		case Action::right:
			return inputManager.IsKeyDown(SDL_SCANCODE_D);
			break;
		case Action::push:
			return inputManager.IsKeyDown(SDL_SCANCODE_SPACE);
			break;
		case Action::nextScene:
			return inputManager.IsKeyDown(SDL_SCANCODE_F1);
			break;
		case Action::switchPlayerCount:
			return (inputManager.IsKeyDown(SDL_SCANCODE_LEFT) || inputManager.IsKeyDown(SDL_SCANCODE_RIGHT));
			break;
		}

		return false;
	}
};


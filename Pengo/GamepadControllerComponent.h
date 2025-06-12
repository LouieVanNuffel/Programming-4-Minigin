#pragma once
#include "ControllerComponent.h"
#include "Controller.h"
#include "Actions.h"

class GamepadControllerComponent final : public dae::ControllerComponent
{
public:
	GamepadControllerComponent(dae::GameObject* gameObject, int controllerIndex)
		:ControllerComponent(gameObject), m_ControllerIndex{ controllerIndex }
	{

	}

private:
	int m_ControllerIndex;

	virtual bool ActionHappened(uint32_t action) override
	{
		auto& controller = dae::Controller::GetInstance();
		Action actionEnum = static_cast<Action>(action);

		switch (actionEnum)
		{
		case Action::up:
			return controller.IsPressed(XINPUT_GAMEPAD_DPAD_UP, m_ControllerIndex);
			break;
		case Action::down:
			return controller.IsPressed(XINPUT_GAMEPAD_DPAD_DOWN, m_ControllerIndex);
			break;
		case Action::left:
			return controller.IsPressed(XINPUT_GAMEPAD_DPAD_LEFT, m_ControllerIndex);
			break;
		case Action::right:
			return controller.IsPressed(XINPUT_GAMEPAD_DPAD_RIGHT, m_ControllerIndex);
			break;
		case Action::push:
			return controller.IsPressed(XINPUT_GAMEPAD_B, m_ControllerIndex);
			break;
		}

		return false;
	}
};


#pragma once
#include "ControllerComponent.h"
#include "Controller.h"

class GamepadControllerComponent final : public dae::ControllerComponent
{
public:
	GamepadControllerComponent(dae::GameObject* gameObject, int controllerIndex)
		:ControllerComponent(gameObject), m_ControllerIndex{ controllerIndex }
	{

	}

private:
	int m_ControllerIndex;

	virtual bool ActionHappened(dae::Action action) override
	{
		auto& controller = dae::Controller::GetInstance();

		switch (action)
		{
		case dae::Action::up:
			return controller.IsPressed(XINPUT_GAMEPAD_DPAD_UP, m_ControllerIndex);
			break;
		case dae::Action::down:
			return controller.IsPressed(XINPUT_GAMEPAD_DPAD_DOWN, m_ControllerIndex);
			break;
		case dae::Action::left:
			return controller.IsPressed(XINPUT_GAMEPAD_DPAD_LEFT, m_ControllerIndex);
			break;
		case dae::Action::right:
			return controller.IsPressed(XINPUT_GAMEPAD_DPAD_RIGHT, m_ControllerIndex);
			break;
		case dae::Action::push:
			return controller.IsPressed(XINPUT_GAMEPAD_B, m_ControllerIndex);
			break;
		}

		return false;
	}
};


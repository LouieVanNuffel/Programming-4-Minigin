#include "Controller.h"
#include <iostream>

void dae::Controller::Update()
{
    for (DWORD controllerIndex = 0; controllerIndex < 1; controllerIndex++) //Currently just one, could be more for multiple controllers?
    {
        CopyMemory(&previousState, &currentState, sizeof(XINPUT_STATE));
        ZeroMemory(&currentState, sizeof(XINPUT_STATE));
        XInputGetState(controllerIndex, &currentState);

        buttonChanges = currentState.Gamepad.wButtons ^ previousState.Gamepad.wButtons;
        buttonsPressedThisFrame = buttonChanges & currentState.Gamepad.wButtons;
        buttonsReleasedThisFrame = buttonChanges & (~currentState.Gamepad.wButtons);
    }
}

bool dae::Controller::IsDownThisFrame(unsigned int button) const
{
    return buttonsPressedThisFrame & button;
}

bool dae::Controller::IsUpThisFrame(unsigned int button) const
{
    return buttonsReleasedThisFrame & button;
}

bool dae::Controller::IsPressed(unsigned int button) const
{
    return currentState.Gamepad.wButtons & button;
}

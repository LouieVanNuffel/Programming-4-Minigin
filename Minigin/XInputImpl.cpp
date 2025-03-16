#pragma once
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include "Xinput.h"

namespace dae
{
	class XInputImpl
	{
	public:
        void Update()
        {
            for (DWORD controllerIndex = 0; controllerIndex < 4; controllerIndex++)
            {
                CopyMemory(&previousState[controllerIndex], &currentState[controllerIndex], sizeof(XINPUT_STATE));
                ZeroMemory(&currentState[controllerIndex], sizeof(XINPUT_STATE));
                XInputGetState(controllerIndex, &currentState[controllerIndex]);

                buttonChanges[controllerIndex] = currentState[controllerIndex].Gamepad.wButtons 
                                                ^ previousState[controllerIndex].Gamepad.wButtons;
                buttonsPressedThisFrame[controllerIndex] = buttonChanges[controllerIndex] 
                                                        & currentState[controllerIndex].Gamepad.wButtons;
                buttonsReleasedThisFrame[controllerIndex] = buttonChanges[controllerIndex] 
                                                        & (~currentState[controllerIndex].Gamepad.wButtons);
            }
        }

        bool IsDownThisFrame(unsigned int button, int controllerIndex) const
        {
            return buttonsPressedThisFrame[controllerIndex] & button;
        }

        bool IsUpThisFrame(unsigned int button, int controllerIndex) const
        {
            return buttonsReleasedThisFrame[controllerIndex] & button;
        }

        bool IsPressed(unsigned int button, int controllerIndex) const
        {
            return currentState[controllerIndex].Gamepad.wButtons & button;
        }

    private:
        XINPUT_STATE previousState[4];
        XINPUT_STATE currentState[4];

        unsigned int buttonChanges[4];
        unsigned int buttonsPressedThisFrame[4];
        unsigned int buttonsReleasedThisFrame[4];
	};
}


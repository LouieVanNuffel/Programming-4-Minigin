#include "Controller.h"
#include <iostream>

void dae::Controller::Update()
{
    m_pXInputImpl->Update();
}

bool dae::Controller::IsDownThisFrame(unsigned int button, int controllerIndex) const
{
    return m_pXInputImpl->IsDownThisFrame(button, controllerIndex);
}

bool dae::Controller::IsUpThisFrame(unsigned int button, int controllerIndex) const
{
    return m_pXInputImpl->IsUpThisFrame(button, controllerIndex);
}

bool dae::Controller::IsPressed(unsigned int button, int controllerIndex) const
{
    return m_pXInputImpl->IsPressed(button, controllerIndex);
}

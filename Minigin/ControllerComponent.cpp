#include "ControllerComponent.h"

void dae::ControllerComponent::Update()
{
	for (const auto& commandActionPair : m_CommandActionBindings)
	{
		if (ActionHappened(commandActionPair.first))
		{
			commandActionPair.second->Execute();
		}
	}
}

void dae::ControllerComponent::BindCommandToAction(std::unique_ptr<Command> command, uint32_t action)
{
	m_CommandActionBindings.emplace(action, std::move(command));
}

void dae::ControllerComponent::ExecuteAction(uint32_t action)
{
	m_CommandActionBindings[action]->Execute();
}

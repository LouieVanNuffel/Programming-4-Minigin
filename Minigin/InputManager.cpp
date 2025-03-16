#include <SDL.h>
#include "InputManager.h"
#include "backends/imgui_impl_sdl2.h"
#include "Controller.h"
#include <iostream>

bool dae::InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			
		}
		// etc...
		
		//process event for IMGUI
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	const Uint8* keystate = SDL_GetKeyboardState(nullptr);
	for (const auto& commandBinding : m_CommandBindingsKeyboard)
	{
		if (keystate[std::get<1>(commandBinding)])
		{
			std::get<0>(commandBinding)->Execute();
		}
	}

	Controller::GetInstance().Update();
	for (const auto& commandBinding : m_CommandBindingsController)
	{
		switch (std::get<2>(commandBinding))
		{
		case PollType::IsDownThisFrame:
			if (Controller::GetInstance().IsDownThisFrame(std::get<1>(commandBinding), std::get<3>(commandBinding)))
			{
				std::get<0>(commandBinding)->Execute();
			}
			break;
		case PollType::IsUpThisFrame:
			if (Controller::GetInstance().IsUpThisFrame(std::get<1>(commandBinding), std::get<3>(commandBinding)))
			{
				std::get<0>(commandBinding)->Execute();
			}
			break;
		case PollType::IsPressed:
			if (Controller::GetInstance().IsPressed(std::get<1>(commandBinding), std::get<3>(commandBinding)))
			{
				std::get<0>(commandBinding)->Execute();
			}
			break;
		}
	}

	return true;
}

void dae::InputManager::BindCommandToController(std::unique_ptr<Command> command, unsigned int button, PollType pollType, int controllerIndex)
{
	m_CommandBindingsController.emplace_back(std::make_tuple(std::move(command), button, pollType, controllerIndex));
}

void dae::InputManager::BindCommandToKeyboard(std::unique_ptr<Command> command, SDL_Scancode scanCode)
{
	m_CommandBindingsKeyboard.emplace_back(std::make_tuple(std::move(command), scanCode));
}

void dae::InputManager::UnbindCommandFromController(std::unique_ptr<Command> command, unsigned int button, PollType pollType, int controllerIndex)
{
	auto it = std::find(
		std::begin(m_CommandBindingsController), 
		std::end(m_CommandBindingsController),
		std::make_tuple(std::move(command), button, pollType, controllerIndex));
	m_CommandBindingsController.erase(it);
}

void dae::InputManager::UnbindCommandFromKeyboard(std::unique_ptr<Command> command, SDL_Scancode scanCode)
{
	auto it = std::find(
		std::begin(m_CommandBindingsKeyboard), 
		std::end(m_CommandBindingsKeyboard),
		std::make_tuple(std::move(command), scanCode));
	m_CommandBindingsKeyboard.erase(it);
}

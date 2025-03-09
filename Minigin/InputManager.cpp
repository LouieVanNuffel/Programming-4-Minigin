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

		for (const auto& commandBinding : m_CommandBindingsKeyboard)
		{
			if (e.type == static_cast<unsigned int>(std::get<2>(commandBinding)))
			{
				if (e.key.keysym.sym == std::get<1>(commandBinding))
				{
					std::get<0>(commandBinding)->Execute();
				}
			}
		}
	}

	Controller::GetInstance().Update();
	for (const auto& commandBinding : m_CommandBindingsController)
	{
		switch (std::get<2>(commandBinding))
		{
		case PollType::IsDownThisFrame:
			if (Controller::GetInstance().IsDownThisFrame(std::get<1>(commandBinding)))
			{
				std::get<0>(commandBinding)->Execute();
			}
			break;
		case PollType::IsUpThisFrame:
			if (Controller::GetInstance().IsUpThisFrame(std::get<1>(commandBinding)))
			{
				std::get<0>(commandBinding)->Execute();
			}
			break;
		case PollType::IsPressed:
			if (Controller::GetInstance().IsPressed(std::get<1>(commandBinding)))
			{
				std::get<0>(commandBinding)->Execute();
			}
			break;
		}
	}

	return true;
}

void dae::InputManager::BindCommandToController(std::unique_ptr<Command> command, unsigned int button, PollType pollType)
{
	m_CommandBindingsController.emplace_back(std::make_tuple(std::move(command), button, pollType));
}

void dae::InputManager::BindCommandToKeyboard(std::unique_ptr<Command> command, SDL_KeyCode keyCode, SDL_EventType eventType)
{
	m_CommandBindingsKeyboard.emplace_back(std::make_tuple(std::move(command), keyCode, eventType));
}

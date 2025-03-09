#pragma once
#include "Singleton.h"
#include "Command.h"
#include "vector"
#include <memory>

namespace dae
{
	enum class PollType
	{
		IsDownThisFrame, IsUpThisFrame, IsPressed
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		void BindCommandToController(std::unique_ptr<Command> command, unsigned int button, PollType pollType);
		void BindCommandToKeyboard(std::unique_ptr<Command> command, SDL_KeyCode keyCode, SDL_EventType eventType);

	private:
		std::vector<std::tuple<std::unique_ptr<Command>, unsigned int, PollType>> m_CommandBindingsController;
		std::vector<std::tuple<std::unique_ptr<Command>, SDL_KeyCode, SDL_EventType>> m_CommandBindingsKeyboard;
	};

}

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
		void BindCommandToController(std::unique_ptr<Command> command, unsigned int button, PollType pollType, int controllerIndex);
		void BindCommandToKeyboard(std::unique_ptr<Command> command, SDL_Scancode scanCode);
		void UnbindCommandFromController(std::unique_ptr<Command> command, unsigned int button, PollType pollType, int controllerIndex);
		void UnbindCommandFromKeyboard(std::unique_ptr<Command> command, SDL_Scancode scanCode);

	private:
		std::vector<std::tuple<std::unique_ptr<Command>, unsigned int, PollType, int>> m_CommandBindingsController;
		std::vector<std::tuple<std::unique_ptr<Command>, SDL_Scancode>> m_CommandBindingsKeyboard;
	};

}

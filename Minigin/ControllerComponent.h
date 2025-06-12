#pragma once
#include "Component.h"
#include <unordered_map>
#include "Command.h"

namespace dae
{
	enum class Action
	{
		up, down, left, right, push, breakBlock, stun
	};

	
	class ControllerComponent : public Component
	{
	public:
		ControllerComponent(GameObject* gameObject)
			: Component(gameObject)
		{

		}

		virtual void Start() {};
		virtual void Update();
		virtual void LateUpdate() {};
		virtual void Render() const {};
		virtual void RenderUI() const {};

		void BindCommandToAction(std::unique_ptr<Command> command, Action action);
		void ExecuteAction(Action action);

	protected:
		virtual bool ActionHappened(Action action) = 0;

	private:
		std::unordered_map<Action, std::unique_ptr<Command>> m_CommandActionBindings;
	};
}
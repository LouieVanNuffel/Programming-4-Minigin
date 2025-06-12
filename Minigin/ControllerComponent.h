#pragma once
#include "Component.h"
#include <unordered_map>
#include "Command.h"

namespace dae
{
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

		void BindCommandToAction(std::unique_ptr<Command> command, uint32_t action);
		void ExecuteAction(uint32_t action);

	protected:
		virtual bool ActionHappened(uint32_t action) = 0;

	private:
		std::unordered_map<uint32_t, std::unique_ptr<Command>> m_CommandActionBindings;
	};
}
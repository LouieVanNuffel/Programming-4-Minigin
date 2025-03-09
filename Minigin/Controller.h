#pragma once
#include "Singleton.h"
#include "Windows.h"
#include "Xinput.h"

namespace dae
{
	class Controller : public Singleton<Controller>
	{
	public:
		void Update();

		bool IsDownThisFrame(unsigned int button) const;
		bool IsUpThisFrame(unsigned int button) const;
		bool IsPressed(unsigned int button) const;

	private:
		XINPUT_STATE previousState;
		XINPUT_STATE currentState;

		unsigned int buttonChanges;
		unsigned int buttonsPressedThisFrame;
		unsigned int buttonsReleasedThisFrame;

	};
}


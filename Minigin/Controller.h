#pragma once
#include "Singleton.h"
#include "XInputImpl.cpp"
#include <memory>

namespace dae
{
	class Controller : public Singleton<Controller>
	{
	public:
		void Update();

		bool IsDownThisFrame(unsigned int button, int controllerIndex) const;
		bool IsUpThisFrame(unsigned int button, int controllerIndex) const;
		bool IsPressed(unsigned int button, int controllerIndex) const;

	private:
		std::unique_ptr<XInputImpl> m_pXInputImpl{ std::make_unique<XInputImpl>() };

	};
}


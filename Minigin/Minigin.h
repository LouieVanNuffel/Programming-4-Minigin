#pragma once
#include <string>
#include <functional>

namespace dae
{
	class Minigin
	{
	public:
		explicit Minigin(const std::string& dataPath);
		~Minigin();
		void Run(const std::function<void()>& Load);

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;

	private:
		const float m_FixedTimeStep{ 1.f / 60.f };
		const int m_MillisecondsPerFrame{ static_cast<int>((1.f / 60.f) * 1000) };
		bool m_HasStarted{ false };
	};
}
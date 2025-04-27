#pragma once
#include "Singleton.h"

namespace dae
{
	class Time : public Singleton<Time>
	{
	public:
		float GetDeltaTime() const
		{
			return m_DeltaTime;
		}

		void SetDeltaTime(float deltaTime)
		{
			m_DeltaTime = deltaTime;
		}

		float GetFixedTime() const
		{
			return m_FixedTime;
		}

		void SetFixedTime(float fixedTime)
		{
			m_FixedTime = fixedTime;
		}

	private:
		float m_DeltaTime;
		float m_FixedTime;
	};
}


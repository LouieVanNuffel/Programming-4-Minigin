#pragma once
#include "EngineTime.h"

class AnimationSequence
{
public:
	AnimationSequence(int offsetPerFrameX, int offsetPerFrameY, int framerate, int amountOfFrames, bool loops = false)
		: m_OffsetPerFrameX{ offsetPerFrameX }, m_OffsetPerFrameY{ offsetPerFrameY },
		m_Framerate{ framerate }, m_AmountOfFrames{ amountOfFrames }, m_Loops{ loops },
		m_TimePerFrame{ 1.0f / static_cast<float>(framerate) }
	{
		
	}

	void Update()
	{
		if (!m_IsAnimating) return;

		m_Timer -= dae::Time::GetInstance().GetDeltaTime();
		if (m_Timer <= 0.0f)
		{
			m_Timer = m_TimePerFrame;

			++m_FrameOffsetX;
			++m_FrameOffsetY;
			if (!m_Loops && m_FrameOffsetX >= m_AmountOfFrames - 1) m_IsAnimating = false;

			m_FrameOffsetX %= m_AmountOfFrames;
			m_FrameOffsetY %= m_AmountOfFrames;
		}
	}

	int GetOffsetX() const { return m_FrameOffsetX * m_OffsetPerFrameX; };
	int GetOffsetY() const { return m_FrameOffsetY * m_OffsetPerFrameY; };

private:
	int m_FrameOffsetX{};
	int m_FrameOffsetY{};
	const int m_OffsetPerFrameX;
	const int m_OffsetPerFrameY;
	const int m_Framerate;
	const int m_AmountOfFrames;
	bool m_Loops;
	bool m_IsAnimating = true;

	const float m_TimePerFrame;
	float m_Timer{};
};


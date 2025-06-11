#pragma once
#include <memory>
#include "AnimationState.h"

namespace dae
{
	class GameObject;
}

enum class PengoColor
{
	red, green, darkYellow, pink, orange, lightYellow, lightBlue, gold, darkBlue
};

class PengoCharacter
{
public:
	//Constructor
	PengoCharacter(PengoColor pengoColor);

	//Destructor
	~PengoCharacter() = default;

	//Rule of 5
	PengoCharacter(const PengoCharacter& other) = default;
	PengoCharacter(PengoCharacter&& other) = default;
	PengoCharacter& operator=(const PengoCharacter& other) = default;
	PengoCharacter& operator=(PengoCharacter&& other) = default;

	std::shared_ptr<dae::GameObject> GetCharacterObject();

private:
	int GetTextureOffsetX(PengoColor pengoColor) const;
	int GetTextureOffsetY(PengoColor pengoColor) const;

	std::shared_ptr<dae::GameObject> m_CharacterObject{};

	AnimationSequenceData m_IdleAnimation{ 16, 0, 8, 1, false };
	AnimationSequenceData m_MovingAnimation{ 16, 0, 8, 2, true };
	AnimationSequenceData m_DeadAnimation{ 16, 0, 8, 2, true };

	AnimationStateData m_IdleStateData{ AnimationStates::idle, m_IdleAnimation, 0, { 4 * 16, 0 * 16, 2 * 16, 6 * 16 } };
	AnimationStateData m_MovingStateData{ AnimationStates::moving, m_MovingAnimation, 0, { 4 * 16, 0 * 16, 2 * 16, 6 * 16 } };
	AnimationStateData m_DeadStateData{ AnimationStates::dead, m_DeadAnimation, 2 * 16, { 0, 0, 0, 0 } };

};


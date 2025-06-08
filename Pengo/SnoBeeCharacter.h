#pragma once
#include <memory>
#include "AnimationState.h"

namespace dae
{
	class GameObject;
}

enum class SnoBeeColor
{
	red, green, darkYellow, pink, orange, lightYellow, lightBlue, gold, darkBlue
};

class SnoBeeCharacter final
{
public:
	//Constructor
	SnoBeeCharacter(SnoBeeColor snoBeeColor);

	//Destructor
	~SnoBeeCharacter() = default;

	//Rule of 5
	SnoBeeCharacter(const SnoBeeCharacter& other) = default;
	SnoBeeCharacter(SnoBeeCharacter&& other) = default;
	SnoBeeCharacter& operator=(const SnoBeeCharacter& other) = default;
	SnoBeeCharacter& operator=(SnoBeeCharacter&& other) = default;

	std::shared_ptr<dae::GameObject> GetCharacterObject();

private:
	int GetTextureOffsetX(SnoBeeColor snoBeeColor) const;
	int GetTextureOffsetY(SnoBeeColor snoBeeColor) const;

	std::shared_ptr<dae::GameObject> m_CharacterObject{};

	AnimationSequenceData m_IdleAnimation{ 16, 0, 8, 1, false };
	AnimationSequenceData m_MovingAnimation{ 16, 0, 8, 2, true };
	AnimationSequenceData m_DeadAnimation{ 16, 0, 8, 2, false };

	AnimationStateData m_IdleStateData{ AnimationStates::idle, m_IdleAnimation, 1 * 16, 1 * 16, { 5 * 16, 1 * 16, 3 * 16, 7 * 16 } };
	AnimationStateData m_MovingStateData{ AnimationStates::moving, m_MovingAnimation, 0, 1 * 16, { 4 * 16, 0 * 16, 2 * 16, 6 * 16 } };
	AnimationStateData m_DeadStateData{ AnimationStates::dead, m_DeadAnimation, 0, 4 * 16, { 4 * 16, 0 * 16, 2 * 16, 6 * 16 } };

};


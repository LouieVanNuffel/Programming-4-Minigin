#pragma once
#include <memory>

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

};


#pragma once
#include <memory>

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

};


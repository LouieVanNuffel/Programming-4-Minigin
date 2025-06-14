#pragma once
#include "TextComponent.h"
#include "GameState.h"

class DisplayPlayerCountComponent final : public dae::TextComponent
{
public:
	DisplayPlayerCountComponent(const std::string& text, std::shared_ptr<dae::Font> font, dae::GameObject* gameObject)
		:dae::TextComponent(text, font, gameObject)
	{

	}

	virtual void Update() override
	{
		TextComponent::Update();
		SetText(std::to_string(GameState::GetInstance().PlayerCount()) + " Players");
	}
};

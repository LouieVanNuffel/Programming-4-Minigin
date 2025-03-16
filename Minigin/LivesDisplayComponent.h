#pragma once
#include "HealthComponent.h"
#include "TextComponent.h"
#include "Observer.h"

namespace dae
{
	class LivesDisplayComponent : public TextComponent, public Observer
	{
	public:
		LivesDisplayComponent(const std::string& text, std::shared_ptr<Font> font, GameObject* gameObject);
		virtual void Notify(const Event& event, const GameObject* gameObject) override;
	};
}


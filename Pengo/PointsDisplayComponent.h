#pragma once
#include "PointComponent.h"
#include "TextComponent.h"
#include "Observer.h"

namespace dae
{
	class PointsDisplayComponent : public TextComponent, public Observer
	{
	public:
		PointsDisplayComponent(const std::string& text, std::shared_ptr<Font> font, GameObject* gameObject);
		virtual void Notify(const Event& event, const GameObject* gameObject) override;
	};
}

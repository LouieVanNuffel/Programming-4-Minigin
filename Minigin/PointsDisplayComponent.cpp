#include "PointsDisplayComponent.h"

dae::PointsDisplayComponent::PointsDisplayComponent(const std::string& text, std::shared_ptr<Font> font, GameObject* gameObject)
	:TextComponent(text, font, gameObject)
{
}

void dae::PointsDisplayComponent::Notify(const Event& event, const GameObject* gameObject)
{
	if (event.id == make_sdbm_hash("PlayerScored"))
	{
		SetText("Score: " + std::to_string(gameObject->GetComponent<PointComponent>()->GetPoints()));
	}
}

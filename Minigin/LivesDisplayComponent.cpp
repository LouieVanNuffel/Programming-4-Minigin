#include "LivesDisplayComponent.h"
#include "GameObject.h"
#include "Event.h"
#include "HealthComponent.h"

dae::LivesDisplayComponent::LivesDisplayComponent(const std::string& text, std::shared_ptr<Font> font, GameObject* gameObject)
	:TextComponent(text, font, gameObject)
{
}

void dae::LivesDisplayComponent::Notify(const Event& event, const GameObject* gameObject)
{
	if (event.id == make_sdbm_hash("PlayerDied"))
	{
		SetText("# lives: " + std::to_string(gameObject->GetComponent<HealthComponent>()->GetLives()));
	}
}

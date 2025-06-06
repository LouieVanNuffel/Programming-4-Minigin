#include "PengoCharacter.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "PointComponent.h"
#include "Subject.h"
#include "VelocityComponent.h"
#include "Animator.h"
#include "BoxColliderComponent.h"

using namespace dae;

PengoCharacter::PengoCharacter(PengoColor pengoColor)
{
	m_CharacterObject = std::make_shared<dae::GameObject>();
	auto healthComponent = std::make_unique<HealthComponent>(m_CharacterObject.get());
	auto pointComponent = std::make_unique<PointComponent>(m_CharacterObject.get());
	auto subjectComponent = std::make_unique<Subject>(m_CharacterObject.get());
	auto velocityComponent = std::make_unique<VelocityComponent>(m_CharacterObject.get());
	auto animator = std::make_unique<Animator>(m_CharacterObject.get());
	subjectComponent->AddObserver(animator.get());
	auto textureComponent = std::make_unique<dae::RenderComponent>(m_CharacterObject.get());
	textureComponent->SetTexture("characterSprites.png", 
								SDL_Rect{ GetTextureOffsetX(pengoColor), GetTextureOffsetY(pengoColor), 16, 16}, 1.5f);
	auto boxColliderComponent = std::make_unique<BoxColliderComponent>(16 * 1.5f, 16 * 1.5f, ObjectType::movable, m_CharacterObject.get());

	m_CharacterObject->AddComponent(std::move(healthComponent));
	m_CharacterObject->AddComponent(std::move(pointComponent));
	m_CharacterObject->AddComponent(std::move(subjectComponent));
	m_CharacterObject->AddComponent(std::move(textureComponent));
	m_CharacterObject->AddComponent(std::move(velocityComponent));
	m_CharacterObject->AddComponent(std::move(animator));
	m_CharacterObject->AddComponent(std::move(boxColliderComponent));
}

std::shared_ptr<dae::GameObject> PengoCharacter::GetCharacterObject()
{
	return m_CharacterObject;
}

int PengoCharacter::GetTextureOffsetX(PengoColor pengoColor) const
{
	switch (pengoColor)
	{
	case PengoColor::red:
		return 0;
		break;
	case PengoColor::green:
		return 1 * 8 * 16;
		break;
	case PengoColor::darkYellow:
		return 2 * 8 * 16;
		break;
	case PengoColor::pink:
		return 3 * 8 * 16;
		break;
	case PengoColor::orange:
		return 0;
		break;
	case PengoColor::lightYellow:
		return 1 * 8 * 16;
		break;
	case PengoColor::lightBlue:
		return 2 * 8 * 16;
		break;
	case PengoColor::gold:
		return 3 * 8 * 16;
		break;
	case PengoColor::darkBlue:
		return 4 * 8 * 16;
		break;
	default:
		return 0;
		break;
	}
}

int PengoCharacter::GetTextureOffsetY(PengoColor pengoColor) const
{
	switch (pengoColor)
	{
	case PengoColor::red:
		return 0;
		break;
	case PengoColor::green:
		return 0;
		break;
	case PengoColor::darkYellow:
		return 0;
		break;
	case PengoColor::pink:
		return 0;
		break;
	case PengoColor::orange:
		return 4 * 16;
		break;
	case PengoColor::lightYellow:
		return 4 * 16;
		break;
	case PengoColor::lightBlue:
		return 4 * 16;
		break;
	case PengoColor::gold:
		return 4 * 16;
		break;
	case PengoColor::darkBlue:
		return 4 * 16;
		break;
	default:
		return 0;
		break;
	}
}

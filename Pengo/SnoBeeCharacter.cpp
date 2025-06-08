#include "SnoBeeCharacter.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "Subject.h"
#include "VelocityComponent.h"
#include "Animator.h"
#include "BoxColliderComponent.h"

using namespace dae;

SnoBeeCharacter::SnoBeeCharacter(SnoBeeColor snoBeeColor)
{
	m_CharacterObject = std::make_shared<dae::GameObject>();
	auto healthComponent = std::make_unique<HealthComponent>(m_CharacterObject.get());
	auto subjectComponent = std::make_unique<Subject>(m_CharacterObject.get());
	auto velocityComponent = std::make_unique<VelocityComponent>(m_CharacterObject.get());
	//auto animator = std::make_unique<Animator>(m_CharacterObject.get());
	//subjectComponent->AddObserver(animator.get());
	auto textureComponent = std::make_unique<dae::RenderComponent>(m_CharacterObject.get());
	textureComponent->SetTexture("characterSprites.png",
		SDL_Rect{ GetTextureOffsetX(snoBeeColor), GetTextureOffsetY(snoBeeColor), 16, 16 }, 1.5f);
	auto boxColliderComponent = std::make_unique<BoxColliderComponent>(16 * 1.5f, 16 * 1.5f, ObjectType::movable, m_CharacterObject.get());

	m_CharacterObject->AddComponent(std::move(healthComponent));
	m_CharacterObject->AddComponent(std::move(subjectComponent));
	m_CharacterObject->AddComponent(std::move(textureComponent));
	m_CharacterObject->AddComponent(std::move(velocityComponent));
	//m_CharacterObject->AddComponent(std::move(animator));
	m_CharacterObject->AddComponent(std::move(boxColliderComponent));
}

std::shared_ptr<dae::GameObject> SnoBeeCharacter::GetCharacterObject()
{
	return m_CharacterObject;
}

int SnoBeeCharacter::GetTextureOffsetX(SnoBeeColor snoBeeColor) const
{
	switch (snoBeeColor)
	{
	case SnoBeeColor::red:
		return 1 * 8 * 16;
		break;
	case SnoBeeColor::green:
		return 0 * 8 * 16;
		break;
	case SnoBeeColor::darkYellow:
		return 2 * 8 * 16;
		break;
	case SnoBeeColor::pink:
		return 3 * 8 * 16;
		break;
	case SnoBeeColor::orange:
		return 0 * 8 * 16;
		break;
	case SnoBeeColor::lightYellow:
		return 1 * 8 * 16;
		break;
	case SnoBeeColor::lightBlue:
		return 2 * 8 * 16;
		break;
	case SnoBeeColor::gold:
		return 3 * 8 * 16;
		break;
	case SnoBeeColor::darkBlue:
		return 4 * 8 * 16;
		break;
	default:
		return 0;
		break;
	}
}

int SnoBeeCharacter::GetTextureOffsetY(SnoBeeColor snoBeeColor) const
{
	switch (snoBeeColor)
	{
	case SnoBeeColor::red:
	case SnoBeeColor::green:
	case SnoBeeColor::darkYellow:
	case SnoBeeColor::pink:
		return 2 * 4 * 16;
		break;
	case SnoBeeColor::orange:
	case SnoBeeColor::lightYellow:
	case SnoBeeColor::lightBlue:
	case SnoBeeColor::gold:
	case SnoBeeColor::darkBlue:
		return 3 * 4 * 16;
		break;
	default:
		return 0;
		break;
	}
}

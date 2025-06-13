#include "SnoBeeCharacter.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "Subject.h"
#include "VelocityComponent.h"
#include "Animator.h"
#include "BoxColliderComponent.h"
#include "SnoBeeComponent.h"
#include "IdleState.h"
#include "MovingState.h"
#include "DeadState.h"
#include "StunnedState.h"
#include "AIControllerComponent.h"
#include "MoveCommand.h"
#include "BreakCommand.h"
#include "Actions.h"
#include "Layers.h"
#include "TileMovementComponent.h"
#include "TilebasedMoveCommand.h"

using namespace dae;

SnoBeeCharacter::SnoBeeCharacter(SnoBeeColor snoBeeColor)
{
	m_CharacterObject = std::make_shared<dae::GameObject>();
	auto healthComponent = std::make_unique<HealthComponent>(m_CharacterObject.get(), 1);
	auto subjectComponent = std::make_unique<Subject>(m_CharacterObject.get());
	auto velocityComponent = std::make_unique<VelocityComponent>(m_CharacterObject.get());
	auto animator = std::make_unique<Animator>(m_CharacterObject.get());
	subjectComponent->AddObserver(animator.get());
	animator->AddState(std::make_unique<IdleState>(animator.get(), m_IdleStateData));
	animator->AddState(std::make_unique<MovingState>(animator.get(), m_MovingStateData));
	animator->AddState(std::make_unique<DeadState>(animator.get(), m_DeadStateData));
	animator->AddState(std::make_unique<StunnedState>(animator.get(), m_StunStateData, 5.0f));

	auto textureComponent = std::make_unique<dae::RenderComponent>(m_CharacterObject.get());
	textureComponent->SetTexture("characterSprites.png",
		SDL_Rect{ GetTextureOffsetX(snoBeeColor), GetTextureOffsetY(snoBeeColor), 16, 16 }, 1.5f);
	auto boxColliderComponent = std::make_unique<BoxColliderComponent>(16 * 1.5f, 16 * 1.5f, ObjectType::movable, 
																	   m_CharacterObject.get(), static_cast<uint32_t>(Layer::snobee));
	auto snoBeeComponent = std::make_unique<SnoBeeComponent>(m_CharacterObject.get(), 16 * 1.5f);
	subjectComponent->AddObserver(snoBeeComponent.get());
	auto tileMovementComponent = std::make_unique<TileMovementComponent>(m_CharacterObject.get(), 50.0f);

	m_CharacterObject->AddComponent(std::move(healthComponent));
	m_CharacterObject->AddComponent(std::move(subjectComponent));
	m_CharacterObject->AddComponent(std::move(textureComponent));
	m_CharacterObject->AddComponent(std::move(velocityComponent));
	m_CharacterObject->AddComponent(std::move(animator));
	m_CharacterObject->AddComponent(std::move(boxColliderComponent));
	m_CharacterObject->AddComponent(std::move(snoBeeComponent));
	m_CharacterObject->AddComponent(std::move(tileMovementComponent));


	// Actions
	auto moveUpCommand = std::make_unique<TileBasedMoveCommand>(m_CharacterObject.get(), 0.f, -1.f);
	auto moveDownCommand = std::make_unique<TileBasedMoveCommand>(m_CharacterObject.get(), 0.f, 1.f);
	auto moveLeftCommand = std::make_unique<TileBasedMoveCommand>(m_CharacterObject.get(), -1.f, .0f);
	auto moveRightCommand = std::make_unique<TileBasedMoveCommand>(m_CharacterObject.get(), 1.f, 0.f);
	auto breakCommand = std::make_unique<BreakCommand>(m_CharacterObject.get());

	auto aiControllerComponent = std::make_unique<AIControllerComponent>(m_CharacterObject.get());
	aiControllerComponent->BindCommandToAction(std::move(moveUpCommand), static_cast<uint32_t>(Action::up));
	aiControllerComponent->BindCommandToAction(std::move(moveDownCommand), static_cast<uint32_t>(Action::down));
	aiControllerComponent->BindCommandToAction(std::move(moveLeftCommand), static_cast<uint32_t>(Action::left));
	aiControllerComponent->BindCommandToAction(std::move(moveRightCommand), static_cast<uint32_t>(Action::right));
	aiControllerComponent->BindCommandToAction(std::move(breakCommand), static_cast<uint32_t>(Action::breakBlock));

	m_CharacterObject->AddComponent(std::move(aiControllerComponent));
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

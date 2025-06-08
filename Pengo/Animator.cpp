#include "Animator.h"
#include "AnimationState.h"
#include "GameObject.h"
#include "Event.h"

using namespace dae;

Animator::Animator(dae::GameObject* gameObject, const AnimationStateData& idleStateData,
	const AnimationStateData& movingStateData, const AnimationStateData& deadStateData)
	:Component(gameObject), m_IdleStateData{ idleStateData }, m_MovingStateData{ movingStateData }, m_DeadStateData{ deadStateData }
{
	m_pAnimationState = std::make_unique<AnimationState>(this, idleStateData);
}

void Animator::Start()
{
	m_pRenderComponent = m_gameObject->GetComponent<dae::RenderComponent>();

	assert(m_pRenderComponent);
}

void Animator::Update()
{
	m_pAnimationState->Update();
};

void Animator::LateUpdate() {};
void Animator::Render() const {};
void Animator::RenderUI() const {};

void Animator::Notify(const Event& event, const dae::GameObject* gameObject)
{
	m_pAnimationState->Notify(event, gameObject);
	std::unique_ptr<AnimationState> newState = LoadNewStateFromEnum(m_pAnimationState->GetNewStateToTransitionTo());
	if (newState != nullptr) EnterNewState(std::move(newState));

	if (event.id == dae::make_sdbm_hash("PlayerMovedLeft"))
	{
		m_Direction = Direction::left;
	}

	if (event.id == dae::make_sdbm_hash("PlayerMovedRight"))
	{
		m_Direction = Direction::right;
	}

	if (event.id == dae::make_sdbm_hash("PlayerMovedUp"))
	{
		m_Direction = Direction::up;
	}

	if (event.id == dae::make_sdbm_hash("PlayerMovedDown"))
	{
		m_Direction = Direction::down;
	}
}

void Animator::SetTexture(std::shared_ptr<Texture2D> texture)
{
	if (m_pRenderComponent != nullptr) m_pRenderComponent->SetTexture(texture);
}

void Animator::AddSourceRectPositionToStartPosition(int x, int y)
{
	if (m_pRenderComponent != nullptr) m_pRenderComponent->AddSourceRectToStartPosition(x, y);
}

const Direction& Animator::GetDirection() const
{
	return m_Direction;
}

void Animator::EnterNewState(std::unique_ptr<AnimationState> newState)
{
	m_pAnimationState->OnExit();
	m_pAnimationState = std::move(newState);
	m_pAnimationState->OnEnter();
}

std::unique_ptr<AnimationState> Animator::LoadNewStateFromEnum(const AnimationStates& animationStateToTransitionTo)
{
	if (m_CurrentAnimationStateEnum == animationStateToTransitionTo) return nullptr;

	switch (animationStateToTransitionTo)
	{
	case AnimationStates::idle:
		m_CurrentAnimationStateEnum = animationStateToTransitionTo;
		return std::make_unique<AnimationState>(this, m_IdleStateData);
		break;
	case AnimationStates::dead:
		m_CurrentAnimationStateEnum = animationStateToTransitionTo;
		return std::make_unique<AnimationState>(this, m_DeadStateData);
		break;
	case AnimationStates::moving:
		m_CurrentAnimationStateEnum = animationStateToTransitionTo;
		return std::make_unique<AnimationState>(this, m_MovingStateData);
		break;
	default:
		return nullptr;
		break;
	}
}

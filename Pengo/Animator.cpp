#include "Animator.h"
#include "AnimationState.h"
#include "IdleState.h"
#include "DeadState.h"
#include "MovingState.h"
#include "GameObject.h"

using namespace dae;

Animator::Animator(dae::GameObject* gameObject)
	:Component(gameObject)
{
	m_pAnimationState = new IdleState(this);
}

Animator::~Animator()
{
	delete m_pAnimationState;
	m_pAnimationState = nullptr;
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
	AnimationState* newState = LoadNewStateFromEnum(m_pAnimationState->GetNewStateToTransitionTo());
	if (newState != nullptr) EnterNewState(newState);
}

void Animator::SetTexture(std::shared_ptr<Texture2D> texture)
{
	if (m_pRenderComponent != nullptr) m_pRenderComponent->SetTexture(texture);
}

void Animator::AddSourceRectPositionToStartPosition(int x, int y)
{
	if (m_pRenderComponent != nullptr) m_pRenderComponent->AddSourceRectToStartPosition(x, y);
}

void Animator::EnterNewState(AnimationState* newState)
{
	m_pAnimationState->OnExit();
	delete m_pAnimationState;
	m_pAnimationState = newState;
	m_pAnimationState->OnEnter();
}

AnimationState* Animator::LoadNewStateFromEnum(const AnimationStates& animationStateToTransitionTo)
{
	if (m_CurrentAnimationStateEnum == animationStateToTransitionTo) return nullptr;

	switch (animationStateToTransitionTo)
	{
	case AnimationStates::idle:
		m_CurrentAnimationStateEnum = animationStateToTransitionTo;
		return new IdleState(this);
		break;
	case AnimationStates::dead:
		m_CurrentAnimationStateEnum = animationStateToTransitionTo;
		return new DeadState(this);
		break;
	case AnimationStates::moving:
		m_CurrentAnimationStateEnum = animationStateToTransitionTo;
		return new MovingState(this);
		break;
	default:
		return nullptr;
		break;
	}
}

#include "Animator.h"
#include "AnimationState.h"
#include "Event.h"
#include "GameObject.h"
#include <cassert>

Animator::Animator(dae::GameObject* gameObject)
	: Component(gameObject)
{
}

void Animator::Start()
{
	m_pRenderComponent = m_gameObject->GetComponent<dae::RenderComponent>();
	assert(m_pRenderComponent);
}

void Animator::Update()
{
	if (m_pCurrentState != nullptr) m_pCurrentState->Update();
}

void Animator::Notify(const dae::Event& event, const dae::GameObject* gameObject)
{
	if (m_pCurrentState != nullptr) m_pCurrentState->Notify(event, gameObject);

	if (event.id == dae::make_sdbm_hash("MovedLeft")) m_Direction = Direction::left;
	if (event.id == dae::make_sdbm_hash("MovedRight")) m_Direction = Direction::right;
	if (event.id == dae::make_sdbm_hash("MovedUp")) m_Direction = Direction::up;
	if (event.id == dae::make_sdbm_hash("MovedDown")) m_Direction = Direction::down;
}

void Animator::AddState(std::unique_ptr<AnimationState> state)
{
	AnimationStates animationType = state->GetStateType();
	m_States[animationType] = std::move(state);

	if (m_pCurrentState == nullptr)
	{
		m_CurrentStateEnum = animationType;
		m_pCurrentState = m_States[animationType].get();
		m_pCurrentState->OnEnter();
	}
}

void Animator::ChangeState(AnimationStates newState)
{
	if (newState == m_CurrentStateEnum || m_States.find(newState) == m_States.end()) return;

	m_pCurrentState->OnExit();
	m_pCurrentState = m_States[newState].get();
	m_CurrentStateEnum = newState;
	m_pCurrentState->OnEnter();
}

const Direction& Animator::GetDirection() const
{
	return m_Direction;
}

void Animator::SetTexture(std::shared_ptr<dae::Texture2D> texture)
{
	if (m_pRenderComponent == nullptr) return;

	m_pRenderComponent->SetTexture(texture);
}

void Animator::AddSourceRectPositionToStartPosition(int x, int y)
{
	if (m_pRenderComponent == nullptr) return;

	m_pRenderComponent->AddSourceRectToStartPosition(x, y);
}

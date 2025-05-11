#include "Animator.h"
#include "AnimationState.h"
#include "IdleState.h"
#include "GameObject.h"

Animator::Animator(dae::GameObject* gameObject)
	:Component(gameObject), m_AnimationState{}, m_pRenderComponent{}
{
	m_AnimationState = new IdleState(this);
}

void Animator::Start()
{
	m_pRenderComponent = m_gameObject->GetComponent<dae::RenderComponent>();
}

void Animator::Update()
{
	m_AnimationState->Update();
};

void Animator::LateUpdate() {};
void Animator::Render() const {};
void Animator::RenderUI() const {};

void Animator::Notify(const Event& event, const dae::GameObject* gameObject)
{
	m_AnimationState->Notify(event, gameObject);
	AnimationState* newState = m_AnimationState->GetNewState();
	if (newState != nullptr) EnterNewState(newState);
}

void Animator::SetTexture(std::shared_ptr<Texture2D> texture)
{
	if (m_pRenderComponent != nullptr) m_pRenderComponent->SetTexture(texture);
}

void Animator::EnterNewState(AnimationState* newState)
{
	m_AnimationState->OnExit();
	m_AnimationState = newState;
	m_AnimationState->OnEnter();
}

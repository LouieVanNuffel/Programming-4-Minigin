#include "Animator.h"
#include "AnimationState.h"
#include "IdleState.h"
#include "GameObject.h"

Animator::Animator(dae::GameObject* gameObject)
	:Component(gameObject), m_pAnimationState{}, m_pRenderComponent{}
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
	AnimationState* newState = m_pAnimationState->GetNewState();
	if (newState != nullptr) EnterNewState(newState);
}

void Animator::SetTexture(std::shared_ptr<Texture2D> texture)
{
	if (m_pRenderComponent != nullptr) m_pRenderComponent->SetTexture(texture);
}

void Animator::EnterNewState(AnimationState* newState)
{
	m_pAnimationState->OnExit();
	delete m_pAnimationState;
	m_pAnimationState = newState;
	m_pAnimationState->OnEnter();
}

#include "FpsComponent.h"
#include "GameObject.h"
#include "Time.h"
#include <format>

using namespace dae;

FpsComponent::FpsComponent(GameObject* gameObject)
	:Component(gameObject){}

void dae::FpsComponent::Start()
{
	m_TextComponent = m_gameObject->GetComponent<TextComponent>();
}

void FpsComponent::Update()
{
	m_FPSCount = 1.f / Time::GetInstance().GetDeltaTime();
	assert(m_TextComponent);
	if (m_TextComponent)
	m_TextComponent->SetText(std::format("FPS: {:.1f}", m_FPSCount));
}

void dae::FpsComponent::LateUpdate(){}

void dae::FpsComponent::Render() const {}

void dae::FpsComponent::RenderUI() const {}

float FpsComponent::GetFPSCount() const
{
	return m_FPSCount;
}

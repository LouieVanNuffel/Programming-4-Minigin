#include "FpsComponent.h"
#include "GameObject.h"
#include <format>

using namespace dae;

FpsComponent::FpsComponent(std::shared_ptr<GameObject> gameObject)
	:Component(gameObject){}

void dae::FpsComponent::Start()
{
	m_TextComponent = m_gameObject.lock()->GetComponent<TextComponent>().get();
}

void FpsComponent::Update(float deltaTime)
{
	m_FPSCount = 1.f / deltaTime;
	assert(m_TextComponent);
	if (m_TextComponent)
	m_TextComponent->SetText(std::format("FPS: {:.1f}", m_FPSCount));
}

void dae::FpsComponent::Render() const {}

float FpsComponent::GetFPSCount() const
{
	return m_FPSCount;
}

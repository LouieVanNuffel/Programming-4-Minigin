#include "RenderComponent.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

using namespace dae;

RenderComponent::RenderComponent(GameObject* gameObject) 
	:Component(gameObject)
{
}

dae::RenderComponent::~RenderComponent()
{
	m_texture.reset();
}

void dae::RenderComponent::Start(){}

void dae::RenderComponent::Update(float){}

void dae::RenderComponent::LateUpdate(float){}

void RenderComponent::Render() const
{
	if (!m_texture)
	{
		return;
	}

	if (auto gameObject = m_gameObject)
	{
		const auto& pos = gameObject->GetWorldPosition();
		Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
	}
}

void dae::RenderComponent::RenderUI() const {}


void RenderComponent::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

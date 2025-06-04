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

void dae::RenderComponent::Update(){}

void dae::RenderComponent::LateUpdate(){}

void RenderComponent::Render() const
{
	if (!m_texture)
	{
		return;
	}

	if (auto gameObject = m_gameObject)
	{
		const auto& pos = gameObject->GetWorldPosition();
		if (m_SourceRect.w == 0 || m_SourceRect.h == 0) Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
		else Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y, m_SourceRect);
	}
}

void dae::RenderComponent::RenderUI() const {}


void RenderComponent::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void RenderComponent::SetTexture(const std::string& filename, const SDL_Rect& sourceRect)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
	SetSourceRect(sourceRect);
}

void dae::RenderComponent::SetTexture(std::shared_ptr<Texture2D> texture)
{
	m_texture = texture;
}

void dae::RenderComponent::SetSourceRect(const SDL_Rect& sourceRect)
{
	SetSourceRectPosition(sourceRect.x, sourceRect.y);
	SetSourceRectSize(sourceRect.w, sourceRect.h);
}

void dae::RenderComponent::AddSourceRectPosition(int x, int y)
{
	SetSourceRectPosition(m_SourceRect.x + x, m_SourceRect.y + y);
}

void dae::RenderComponent::SetSourceRectPosition(int x, int y)
{
	assert(x >= 0 && y >= 0
		&& x <= m_texture->GetSize().x && y <= m_texture->GetSize().y);

	m_SourceRect.x = x;
	m_SourceRect.y = y;
}

void dae::RenderComponent::SetSourceRectSize(int w, int h)
{
	assert(w >= 0 && h >= 0
		&& w <= m_texture->GetSize().x && h <= m_texture->GetSize().y);

	m_SourceRect.w = w;
	m_SourceRect.h = h;
}

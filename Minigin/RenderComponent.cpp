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
		if (m_SourceRect.w == 0 || m_SourceRect.h == 0) Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y, m_Scale);
		else Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y, m_SourceRect, m_Scale);
	}
}

void dae::RenderComponent::RenderUI() const {}


void RenderComponent::SetTexture(const std::string& filename, float scale)
{
	m_Scale = scale;
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void RenderComponent::SetTexture(const std::string& filename, const SDL_Rect& sourceRect, float scale)
{
	m_Scale = scale;
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
	SetSourceRect(sourceRect);
	SetStartSourceRect(sourceRect);
}

void dae::RenderComponent::SetTexture(std::shared_ptr<Texture2D> texture, float scale)
{
	m_Scale = scale;
	m_texture = texture;
}

void dae::RenderComponent::SetSourceRect(const SDL_Rect& sourceRect)
{
	SetSourceRectPosition(sourceRect.x, sourceRect.y, m_SourceRect);
	SetSourceRectSize(sourceRect.w, sourceRect.h, m_SourceRect);
}

void dae::RenderComponent::SetStartSourceRect(const SDL_Rect& sourceRect)
{
	SetSourceRectPosition(sourceRect.x, sourceRect.y, m_StartSourceRect);
	SetSourceRectSize(sourceRect.w, sourceRect.h, m_StartSourceRect);
}

void dae::RenderComponent::AddSourceRectPosition(int x, int y)
{
	SetSourceRectPosition(m_SourceRect.x + x, m_SourceRect.y + y, m_SourceRect);
}

void dae::RenderComponent::AddSourceRectToStartPosition(int x, int y)
{
	SetSourceRectPosition(m_StartSourceRect.x + x, m_StartSourceRect.y + y, m_SourceRect);
}

void dae::RenderComponent::SetSourceRectPosition(int x, int y, SDL_Rect& sourceRect)
{
	assert(x >= 0 && y >= 0
		&& x <= m_texture->GetSize().x && y <= m_texture->GetSize().y);

	sourceRect.x = x;
	sourceRect.y = y;
}

void dae::RenderComponent::SetSourceRectSize(int w, int h, SDL_Rect& sourceRect)
{
	assert(w >= 0 && h >= 0
		&& w <= m_texture->GetSize().x && h <= m_texture->GetSize().y);

	sourceRect.w = w;
	sourceRect.h = h;
}

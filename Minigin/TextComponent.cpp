#include <stdexcept>
#include <SDL_ttf.h>
#include "TextComponent.h"

#include "GameObject.h"
#include "Renderer.h"

using namespace dae;

TextComponent::TextComponent(const std::string& text, std::shared_ptr<Font> font, GameObject* gameObject)
	:Component(gameObject), m_needsUpdate(true), m_text(text), m_font(std::move(font)), m_textTexture(nullptr)
{}

TextComponent::~TextComponent()
{
	m_textTexture.reset();
	m_font.reset();
}

void dae::TextComponent::Start(){}

void TextComponent::Update()
{
	if (m_needsUpdate)
	{
		const SDL_Color color = { 255,255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_textTexture = std::make_shared<Texture2D>(texture);
		m_needsUpdate = false;
	}
}

void dae::TextComponent::LateUpdate(){}

void TextComponent::Render() const
{
	if (auto gameObject = m_gameObject)
	{
		const auto& pos = gameObject->GetTransform().GetPosition();
		Renderer::GetInstance().RenderTexture(*m_textTexture, pos.x, pos.y);
	}
}

void dae::TextComponent::RenderUI() const{}

// This implementation uses the "dirty flag" pattern
void TextComponent::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}


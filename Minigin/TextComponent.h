#pragma once
#include <string>
#include <memory>

#include "Component.h"
#include "Font.h"
#include "Texture2D.h"

namespace dae
{
	class GameObject;

	class TextComponent : public Component
	{
	public:

		//Constructor
		TextComponent(const std::string& text, std::shared_ptr<Font> font, GameObject* gameObject);

		//Destructor
		virtual ~TextComponent() override;

		//Rule of 5
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

		virtual void Start() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() const override;
		virtual void RenderUI() const override;

		virtual void SetText(const std::string& text);
	private:

		bool m_needsUpdate;
		std::string m_text;
		std::shared_ptr<Font> m_font;
		std::shared_ptr<Texture2D> m_textTexture;
	};
}

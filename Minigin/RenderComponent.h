#pragma once
#include <memory>

#include "Component.h"
#include "Texture2D.h"
#include <SDL_rect.h>

namespace dae
{
	class GameObject;

	class RenderComponent final : public Component
	{
	public:
		//Constructor
		RenderComponent(GameObject* gameObject);

		//Destructor
		virtual ~RenderComponent() override;

		// Rule Of Five
		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;

		virtual void Start() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() const override;
		virtual void RenderUI() const override;

		void SetTexture(const std::string& filename, float scale = 1.0f);
		void SetTexture(const std::string& filename, const SDL_Rect& sourceRect, float scale = 1.0f);
		void SetTexture(std::shared_ptr<Texture2D> texture, float scale = 1.0f);

		void SetSourceRect(const SDL_Rect& sourceRect);
		void SetStartSourceRect(const SDL_Rect& sourceRect);
		void SetSourceRectPosition(int x, int y, SDL_Rect& sourceRect);
		void AddSourceRectPosition(int x, int y);
		void AddSourceRectToStartPosition(int x, int y);
		void SetSourceRectSize(int w, int h, SDL_Rect& sourceRect);

	private:
		SDL_Rect m_SourceRect{};
		SDL_Rect m_StartSourceRect{};
		float m_Scale{};
		std::shared_ptr<Texture2D> m_texture{ nullptr };
	};
}


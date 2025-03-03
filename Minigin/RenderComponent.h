#pragma once
#include <memory>

#include "Component.h"
#include "Texture2D.h"

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
		virtual void Update(float deltaTime) override;
		virtual void LateUpdate(float deltaTime) override;
		virtual void Render() const override;
		virtual void RenderUI() const override;

		void SetTexture(const std::string& filename);

	private:

		std::shared_ptr<Texture2D> m_texture{ nullptr };
	};
}


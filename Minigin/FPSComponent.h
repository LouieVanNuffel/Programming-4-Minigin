#pragma once
#include "Component.h"
#include "TextComponent.h"

namespace dae
{
	class GameObject;
	class FpsComponent final : public Component
	{
	public:
		//Constructor
		FpsComponent(GameObject* gameObject);

		//Destructor
		virtual ~FpsComponent() override = default;

		//Rule of 5
		FpsComponent(const FpsComponent& other) = default;
		FpsComponent(FpsComponent&& other) = default;
		FpsComponent& operator=(const FpsComponent& other) = default;
		FpsComponent& operator=(FpsComponent&& other) = default;

		virtual void Start() override;
		virtual void Update(float deltaTime) override;
		virtual void LateUpdate(float deltaTime) override;
		virtual void Render() const override;
		float GetFPSCount() const;

	private:
		float m_FPSCount{};
		TextComponent* m_TextComponent{};
	};
}


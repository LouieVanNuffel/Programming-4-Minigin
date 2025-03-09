#pragma once
#include "Component.h"

namespace dae
{
	class GameObject;
	class MoveComponent final : public Component
	{
	public:
		//Constructor
		MoveComponent(GameObject* gameObject, float speed, bool isContinuous = false);

		//Destructor
		virtual ~MoveComponent() override;

		// Rule Of Five
		MoveComponent(const MoveComponent& other) = delete;
		MoveComponent(MoveComponent&& other) = delete;
		MoveComponent& operator=(const MoveComponent& other) = delete;
		MoveComponent& operator=(MoveComponent&& other) = delete;

		virtual void Start() override;
		virtual void Update(float deltaTime) override;
		virtual void LateUpdate(float deltaTime) override;
		virtual void Render() const override;
		virtual void RenderUI() const override;

		void Move(float directionX, float directionY);

		void SetUpPressed(bool value);
		void SetDownPressed(bool value);
		void SetLeftPressed(bool value);
		void SetRightPressed(bool value);


	private:
		bool m_IsContinuous{};
		float m_Speed{};
		float m_DeltaTime{};

		bool m_Up{};
		bool m_Down{};
		bool m_Left{};
		bool m_Right{};
	};
}


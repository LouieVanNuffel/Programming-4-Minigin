#pragma once
#include "Component.h"

namespace dae
{
	class GameObject;
	class RotationComponent final : public Component
	{
	public:
		//Constructor
		RotationComponent(GameObject* gameObject, float rotationSpeedPerSecond, float rotationCircleRadius);

		//Destructor
		virtual ~RotationComponent() override = default;

		//Rule of 5
		RotationComponent(const RotationComponent& other) = default;
		RotationComponent(RotationComponent&& other) = default;
		RotationComponent& operator=(const RotationComponent& other) = default;
		RotationComponent& operator=(RotationComponent&& other) = default;

		virtual void Start() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() const override;
		virtual void RenderUI() const override;

	private:
		float m_currentRotation{};
		const float m_rotationSpeedPerSecond{};
		const float m_rotationCircleRadius{};
	};
}


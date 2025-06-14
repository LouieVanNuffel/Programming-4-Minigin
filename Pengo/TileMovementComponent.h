#pragma once
#include "Component.h"
#include <glm.hpp>

class TileMovementComponent final : public dae::Component
{
public:
	TileMovementComponent(dae::GameObject* gameObject, float speed);

	virtual void Start() override;
	virtual void Update() override;
	virtual void LateUpdate() override {};
	virtual void Render() const override {};
	virtual void RenderUI() const override {};

	void SetDirection(float directionX, float directionY);
	void Reset();

private:
	void Normalize(glm::vec3& vector);

	glm::vec3 m_TargetPosition{};
	float m_Speed;
	bool m_HasArrived{ false };

};


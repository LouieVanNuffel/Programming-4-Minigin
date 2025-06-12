#pragma once
#include "Component.h"

class WallComponent final : public dae::Component
{
public:
	WallComponent(dae::GameObject* gameObject)
		:dae::Component(gameObject)
	{

	}

	virtual void Start() {};
	virtual void Update();
	virtual void LateUpdate() {};
	virtual void Render() const {};
	virtual void RenderUI() const {};
	virtual void OnCollisionEnter(const dae::BoxColliderComponent& other) override;

	void Push();

private:
	bool m_IsVibrating{ false };
	float m_VibrationTime{ 1.0f };
	float m_Timer{};
};


#pragma once
#include "Component.h"

class SnoBeeComponent final : public dae::Component
{
public:
	//Constructor
	SnoBeeComponent(dae::GameObject* gameObject);

	//Destructor
	virtual ~SnoBeeComponent() override = default;

	//Rule of 5
	SnoBeeComponent(const SnoBeeComponent& other) = delete;
	SnoBeeComponent(SnoBeeComponent&& other) = delete;
	SnoBeeComponent& operator=(const SnoBeeComponent& other) = delete;
	SnoBeeComponent& operator=(SnoBeeComponent&& other) = delete;

	virtual void Start() override;
	virtual void Update() override;
	virtual void LateUpdate() override {};
	virtual void Render() const override {};
	virtual void RenderUI() const override {};
	virtual void OnCollisionEnter(const dae::BoxColliderComponent& other);
	
private:


};


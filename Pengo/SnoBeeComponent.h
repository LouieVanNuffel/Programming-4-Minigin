#pragma once
#include "Component.h"
#include "Observer.h"
#include "glm.hpp"

namespace dae
{
	class Subject;
	class VelocityComponent;
	class BoxColliderComponent;
	struct HitInfo;
	struct Event;
}

class SnoBeeComponent final : public dae::Component, public dae::Observer
{
public:
	//Constructor
	SnoBeeComponent(dae::GameObject* gameObject, float blockSize, float speed = 50.0f, float chaseRange = 75.0f, float spawnDelay = 2.0f);

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
	virtual void OnDestroy() override;

	void Stun();

	virtual void Notify(const dae::Event& event, const dae::GameObject* gameObject) override;
	
private:
	enum class MoveDirection
	{
		up, down, left, right
	};

	enum class BehaviorState
	{
		patrol, chase
	};

	void SetDirection(MoveDirection direction);
	void SetRandomDirection();
	bool PerformRaycast(dae::HitInfo& hitInfo_Out) const;

	void Patrol();
	void Chase();
	float DistanceToObjectSquared(dae::GameObject* gameObject) const;
	glm::vec2 DirectionToTarget() const;

	dae::GameObject* m_pTargetObject{ nullptr };
	dae::BoxColliderComponent* m_pBoxColliderComponent{ nullptr };
	dae::Subject* m_pSubjectComponent{ nullptr };
	MoveDirection m_Direction{ MoveDirection::up };
	BehaviorState m_CurrentBehaviorState{ BehaviorState::patrol };

	int m_DirectionX{};
	int m_DirectionY{};
	float m_BlockSize;
	float m_Speed;
	float m_ChaseRange;
	float m_SpawnTimer;
	float m_StunTimer{};
	float m_StunTime{ 5.0f };

	bool m_HasSpawned{ false };
	bool m_IsStunned{ false };

};


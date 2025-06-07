#pragma once
#include <vector>
#include "Singleton.h"

namespace dae
{
	class BoxColliderComponent;
	struct BoxCollider;

	struct HitInfo
	{
		float x;
		float y;
		const BoxColliderComponent* other = nullptr;
	};

	struct Raycast
	{
		float startX;
		float startY;
		int directionX;
		int directionY;
		float distance;
	};

	class CollisionSystem final : public Singleton<CollisionSystem>
	{
	public:
		//Constructor
		CollisionSystem();

		//Destructor
		~CollisionSystem() = default;

		//Rule of 5
		CollisionSystem(const CollisionSystem& other) = delete;
		CollisionSystem(CollisionSystem&& other) = delete;
		CollisionSystem& operator=(const CollisionSystem& other) = delete;
		CollisionSystem& operator=(CollisionSystem&& other) = delete;

		void AddCollider(BoxColliderComponent* boxColliderComponent);
		void RemoveCollider(BoxColliderComponent* boxColliderComponent);

		bool PerformRaycast(const Raycast& raycast, HitInfo& hitInfo_Out, const BoxColliderComponent* colliderToIgnore = nullptr);

		void Update();

	private:
		bool AreColliding(const BoxCollider& box1, const BoxCollider& box2) const;
		bool AreColliding(float positionX, float positionY, const BoxCollider& box) const;

		float OverlapX(const BoxCollider& box1, const BoxCollider& box2) const;
		float OverlapY(const BoxCollider& box1, const BoxCollider& box2) const;

		int DirectionX(const BoxCollider& box1, const BoxCollider& box2) const;
		int DirectionY(const BoxCollider& box1, const BoxCollider& box2) const;

		float DistanceX(const BoxCollider& box1, const BoxCollider& box2) const;
		float DistanceY(const BoxCollider& box1, const BoxCollider& box2) const;

		bool MovedThisFrame(const BoxColliderComponent* boxColliderComponent) const;

		void MoveColliders(BoxColliderComponent* boxColliderComponent1, BoxColliderComponent* boxColliderComponent2);

		std::vector<BoxColliderComponent*> m_BoxColliderComponents{};

	};
}


#pragma once
#include "Component.h"
#include <glm.hpp>

namespace dae
{
	struct BoxCollider
	{
		float x{};
		float y{};
		float width{};
		float height{};
		float centerX{};
		float centerY{};
	};

	enum class ObjectType
	{
		immovable, movable
	};

	class VelocityComponent;
	class GameObject;
	class BoxColliderComponent final : public Component
	{
	public:
		//Constructor
		BoxColliderComponent(float width, float height, ObjectType objectType, GameObject* gameObject);

		//Destructor
		virtual ~BoxColliderComponent() override = default;

		//Rule of 5
		BoxColliderComponent(const BoxColliderComponent& other) = delete;
		BoxColliderComponent(BoxColliderComponent&& other) = delete;
		BoxColliderComponent& operator=(const BoxColliderComponent& other) = delete;
		BoxColliderComponent& operator=(BoxColliderComponent&& other) = delete;

		virtual void Start() override;
		virtual void Update() override;
		virtual void LateUpdate() override {};
		virtual void Render() const override {};
		virtual void RenderUI() const override {};

		void Move(float x, float y, ObjectType objectType = ObjectType::immovable);
		void CallOnCollisionEnter(const BoxColliderComponent& other);
		const BoxCollider& BoxDimensions() const;
		const ObjectType& GetObjectType() const;
		const glm::vec3& Velocity() const;

	private:
		void UpdatePosition();

		BoxCollider m_BoxDimensions{};
		ObjectType m_ObjectType;
		VelocityComponent* m_pVelocityComponent{ nullptr };

	};
}
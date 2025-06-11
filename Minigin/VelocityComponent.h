#pragma once
#include "Component.h"
#include "glm.hpp"

// This component just tracks the velocity of a gameobject and notifies observers about the current velocity

namespace dae
{
	class Subject;
	class GameObject;
	class VelocityComponent final : public Component
	{
	public:

		//Constructor
		VelocityComponent(GameObject* gameObject);

		//Destructor
		virtual ~VelocityComponent() override;

		//Rule of 5
		VelocityComponent(const VelocityComponent& other) = delete;
		VelocityComponent(VelocityComponent&& other) = delete;
		VelocityComponent& operator=(const VelocityComponent& other) = delete;
		VelocityComponent& operator=(VelocityComponent&& other) = delete;

		virtual void Start() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() const override;
		virtual void RenderUI() const override;

		const glm::vec3& GetVelocity() const;
		void AddCollisionsOffset(float x, float y);

	private:
		void NotifyAboutDirection() const;

		enum class Direction
		{
			up, down, left, right, none
		};

		Direction m_Direction{ Direction::none };
		Direction m_DirectionLastFrame{ Direction::none };

		glm::vec3 m_Velocity{};
		glm::vec3 m_PositionLastFrame{};
		glm::vec3 m_PositionThisFrame{};
		// This gets set when position changes because of collision detection with an immovable object
		// We don't want these position changes to influence our velocity calculation
		glm::vec3 m_CollisionsOffset{};
		Subject* m_pSubject{ nullptr };
	};
}

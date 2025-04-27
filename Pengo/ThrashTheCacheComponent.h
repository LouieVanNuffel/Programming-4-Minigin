#pragma once
#include "Component.h"
#include <vector>

namespace dae
{
	struct TransformStruct
	{
		float matrix[16] = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1 };
	};

	class GameObject3D
	{
	public:
		TransformStruct transform;
		int ID;
	};

	class GameObject3DAlt
	{
	public:
		TransformStruct* transform;
		int ID;
	};

	class GameObject;
	class ThrashTheCacheComponent : public Component
	{
	public:
		//Constructor
		ThrashTheCacheComponent(GameObject* gameObject);

		//Destructor
		virtual ~ThrashTheCacheComponent() override = default;

		//Rule of 5
		ThrashTheCacheComponent(const ThrashTheCacheComponent& other) = default;
		ThrashTheCacheComponent(ThrashTheCacheComponent&& other) = default;
		ThrashTheCacheComponent& operator=(const ThrashTheCacheComponent& other) = default;
		ThrashTheCacheComponent& operator=(ThrashTheCacheComponent&& other) = default;

		virtual void Start() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() const override;
		virtual void RenderUI() const override;

	private:
		std::vector<float> TestForIntArray(const int arrayLength, const int sampleSize) const;
		std::vector<float> TestForGameObject3DArray(const int arrayLength, const int sampleSize) const;
		std::vector<float> TestForGameObject3DAltArray(const int arrayLength, const int sampleSize) const;
		float GetAverage(std::vector<float>& durationTimes) const;
	};
}


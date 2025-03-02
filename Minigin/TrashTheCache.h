#pragma once
#include <vector>

namespace dae
{
	class TrashTheCache
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

		std::vector<float> TestForIntArray(const int arrayLength, const int sampleSize) const;
		std::vector<float> TestForGameObject3DArray(const int arrayLength, const int sampleSize) const;
		std::vector<float> TestForGameObject3DAltArray(const int arrayLength, const int sampleSize) const;
		float GetAverage(std::vector<float>& durationTimes) const;

	public:
		void RenderTrashTheCacheWindows() const;
	};
}


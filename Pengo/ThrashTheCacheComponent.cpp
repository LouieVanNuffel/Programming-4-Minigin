#include "ThrashTheCacheComponent.h"
#include <iostream>
#include <chrono>
#include <algorithm>
#include "imgui_plot.h"

dae::ThrashTheCacheComponent::ThrashTheCacheComponent(GameObject* gameObject)
	:Component(gameObject)
{
}

void dae::ThrashTheCacheComponent::Start()
{
}

void dae::ThrashTheCacheComponent::Update()
{
}

void dae::ThrashTheCacheComponent::LateUpdate()
{
}

void dae::ThrashTheCacheComponent::Render() const
{
}

void dae::ThrashTheCacheComponent::RenderUI() const
{
	const int arrayLength{ static_cast<int>(std::pow(2,26)) };

	// Set base values for plot
	ImGui::PlotConfig conf;
	conf.scale.min = 0.0f;
	conf.tooltip.show = true;
	conf.tooltip.format = "x=%.2f, y=%.2f";
	conf.grid_x.show = false;
	conf.grid_y.show = true;
	conf.frame_size = ImVec2(200, 100);
	conf.line_thickness = 2.f;

	// Exercise 1 window
	static int exercise1_samples = 10;

	static std::vector<float> exercise1_results{};

	ImGui::Begin("Exercise 1");
	ImGui::Text("Samples:");

	// Display the integer field
	ImGui::SameLine();
	ImGui::InputInt("##exercise1", &exercise1_samples);

	// Int array
	if (ImGui::Button("Thrash the Cache"))
		exercise1_results = TestForIntArray(arrayLength, exercise1_samples);

	if (!exercise1_results.empty())
	{
		conf.values.ys = exercise1_results.data();
		conf.values.count = static_cast<int>(exercise1_results.size());
		conf.scale.max = exercise1_results[0];
		conf.values.color = ImColor(0, 0, 255);
		ImGui::Plot("IntResults", conf);
	}

	ImGui::End();

	///////////////////////////////////////////////////////////

	// Exercise 2 window
	static int exercise2_samples = 5;

	static std::vector<float> exercise2_GameObject3D_results{};
	static std::vector<float> exercise2_GameObject3DAlt_results{};

	ImGui::Begin("Exercise 2");
	ImGui::Text("Samples:");

	// Display the integer field
	ImGui::SameLine();
	ImGui::InputInt("##exercise2", &exercise2_samples);

	// GameObject3D
	if (ImGui::Button("Thrash the Cache With GameObject3D"))
		exercise2_GameObject3D_results = TestForGameObject3DArray(arrayLength, exercise2_samples);

	if (!exercise2_GameObject3D_results.empty())
	{
		conf.values.ys = exercise2_GameObject3D_results.data();
		conf.values.count = static_cast<int>(exercise2_GameObject3D_results.size());
		conf.scale.max = exercise2_GameObject3D_results[0];
		conf.values.color = ImColor(0, 255, 0);
		ImGui::Plot("GameObject3DResults", conf);
	}

	// GameObject3DAlt
	if (ImGui::Button("Thrash the Cache With GameObject3DAlt"))
		exercise2_GameObject3DAlt_results = TestForGameObject3DAltArray(arrayLength, exercise2_samples);

	if (!exercise2_GameObject3DAlt_results.empty())
	{
		conf.values.ys = exercise2_GameObject3DAlt_results.data();
		conf.values.count = static_cast<int>(exercise2_GameObject3DAlt_results.size());
		conf.scale.max = exercise2_GameObject3DAlt_results[0];
		conf.values.color = ImColor(255, 0, 0);
		ImGui::Plot("GameObject3DAltResults", conf);
	}

	// Combined
	if (!exercise2_GameObject3D_results.empty() && !exercise2_GameObject3DAlt_results.empty())
	{
		ImGui::Text("Combined:");

		static const float* y_data[] = { exercise2_GameObject3D_results.data(), exercise2_GameObject3DAlt_results.data() };
		static ImU32 colors[3] = { ImColor(0, 255, 0), ImColor(255, 0, 0) };

		// Draw plot with multiple sources
		ImGui::PlotConfig conf2;
		conf2.values.count = static_cast<int>(exercise2_GameObject3D_results.size());
		conf2.values.ys_list = y_data;
		conf2.values.ys_count = 2;
		conf2.values.colors = colors;
		conf2.scale.min = 0;
		conf2.scale.max = exercise2_GameObject3D_results[0];
		conf2.tooltip.show = true;
		conf2.frame_size = ImVec2(200, 100);
		ImGui::Plot("Combined", conf2);
	}

	ImGui::End();
}

std::vector<float> dae::ThrashTheCacheComponent::TestForIntArray(const int arrayLength, const int sampleSize) const
{
	std::vector<float> results{};

	if (arrayLength <= 0) return results;

	results.reserve(11);

	int* arrayOfInt = new int[arrayLength] {};

	std::vector<float> durationTimes{};
	durationTimes.reserve(sampleSize);

	for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
	{
		durationTimes.clear();

		for (int sampleIndex{}; sampleIndex < sampleSize; ++sampleIndex)
		{
			auto startTime = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < arrayLength; i += stepsize)
			{
				arrayOfInt[i] *= 2;
			}
			auto endTime = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
			durationTimes.emplace_back(static_cast<float>(duration));
		}

		results.emplace_back(GetAverage(durationTimes));
	}

	delete[] arrayOfInt;

	return results;
}

std::vector<float> dae::ThrashTheCacheComponent::TestForGameObject3DArray(const int arrayLength, const int sampleSize) const
{
	std::vector<float> results{};

	if (arrayLength <= 0) return results;

	results.reserve(11);

	GameObject3D* arrayOfGameObject3D = new GameObject3D[arrayLength]{};

	std::vector<float> durationTimes{};
	durationTimes.reserve(sampleSize);

	for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
	{
		durationTimes.clear();

		for (int sampleIndex{}; sampleIndex < sampleSize; ++sampleIndex)
		{
			auto startTime = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < arrayLength; i += stepsize)
			{
				arrayOfGameObject3D[i].ID *= 2;
			}
			auto endTime = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
			durationTimes.emplace_back(static_cast<float>(duration));
		}

		results.emplace_back(GetAverage(durationTimes));
	}

	delete[] arrayOfGameObject3D;

	return results;
}

std::vector<float> dae::ThrashTheCacheComponent::TestForGameObject3DAltArray(const int arrayLength, const int sampleSize) const
{
	std::vector<float> results{};

	if (arrayLength <= 0) return results;

	results.reserve(11);

	GameObject3DAlt* arrayOfGameObject3DAlt = new GameObject3DAlt[arrayLength]{};

	std::vector<float> durationTimes{};
	durationTimes.reserve(sampleSize);

	for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
	{
		durationTimes.clear();

		for (int sampleIndex{}; sampleIndex < sampleSize; ++sampleIndex)
		{
			auto startTime = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < arrayLength; i += stepsize)
			{
				arrayOfGameObject3DAlt[i].ID *= 2;
			}
			auto endTime = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
			durationTimes.emplace_back(static_cast<float>(duration));
		}

		results.emplace_back(GetAverage(durationTimes));
	}

	delete[] arrayOfGameObject3DAlt;

	return results;
}

float dae::ThrashTheCacheComponent::GetAverage(std::vector<float>& durationTimes) const
{
	float avgTime{};

	if (durationTimes.size() > 4)
	{
		std::sort(durationTimes.begin(), durationTimes.end());
		for (int index{ 1 }; index < static_cast<int>(durationTimes.size() - 1); ++index)
		{
			avgTime += durationTimes[index];
		}
		avgTime /= (durationTimes.size() - 2);
	}
	else
	{
		for (float time : durationTimes)
		{
			avgTime += time;
		}
		avgTime /= durationTimes.size();
	}

	return avgTime;
}

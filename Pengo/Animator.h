#pragma once
#include "Component.h"
#include "Observer.h"
#include "RenderComponent.h"
#include "AnimationState.h"
#include <memory>
#include <unordered_map>

enum class Direction
{
	up, down, left, right
};

namespace dae 
{ 
	struct Event;
}

class Animator final : public dae::Component, public dae::Observer
{
public:
	Animator(dae::GameObject* gameObject);
	~Animator() override = default;

	void Start() override;
	void Update() override;
	void LateUpdate() override {}
	void Render() const override {}
	void RenderUI() const override {}

	void Notify(const dae::Event& event, const dae::GameObject* gameObject) override;

	void AddState(std::unique_ptr<AnimationState> state);
	void ChangeState(AnimationStates newState);

	const Direction& GetDirection() const;
	void SetTexture(std::shared_ptr<dae::Texture2D> texture);
	void AddSourceRectPositionToStartPosition(int x, int y);

private:
	std::unordered_map<AnimationStates, std::unique_ptr<AnimationState>> m_States;
	AnimationState* m_pCurrentState{ nullptr };
	AnimationStates m_CurrentStateEnum = AnimationStates::idle;

	Direction m_Direction{ Direction::down };
	dae::RenderComponent* m_pRenderComponent{ nullptr };
};

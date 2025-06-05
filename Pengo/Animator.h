#pragma once
#include "Component.h"
#include "Observer.h"
#include "RenderComponent.h"
#include "AnimationState.h"

class AnimationState;
class Animator final : public dae::Component, public dae::Observer
{
public:
	Animator(dae::GameObject* gameObject);
	~Animator();

	virtual void Start() override;

	virtual void Update() override;

	virtual void LateUpdate() override;
	virtual void Render() const override;
	virtual void RenderUI() const override;

	virtual void Notify(const dae::Event& event, const dae::GameObject* gameObject) override;

	void SetTexture(std::shared_ptr<dae::Texture2D> texture);
	void AddSourceRectPositionToStartPosition(int x, int y);

private:
	AnimationState* m_pAnimationState{ nullptr };
	AnimationStates m_CurrentAnimationStateEnum = AnimationStates::idle;
	dae::RenderComponent* m_pRenderComponent{ nullptr };

	void EnterNewState(AnimationState* newState);
	AnimationState* LoadNewStateFromEnum(const AnimationStates& animationStateToTransitionTo);
};

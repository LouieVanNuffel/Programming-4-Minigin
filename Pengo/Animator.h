#pragma once
#include "Component.h"
#include "Observer.h"
#include "RenderComponent.h"

using namespace dae;
class AnimationState;
class Animator final : public Component, public Observer
{
public:
	Animator(GameObject* gameObject);

	virtual void Start() override;

	virtual void Update() override;

	virtual void LateUpdate() override;
	virtual void Render() const override;
	virtual void RenderUI() const override;

	virtual void Notify(const Event& event, const GameObject* gameObject) override;

	void SetTexture(std::shared_ptr<Texture2D> texture);

private:
	AnimationState* m_AnimationState;
	RenderComponent* m_pRenderComponent;

	void EnterNewState(AnimationState* newState);
};

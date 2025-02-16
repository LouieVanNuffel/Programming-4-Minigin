#include "FpsComponent.h"
#include <format>


using namespace dae;

FpsComponent::FpsComponent(const std::string& text, std::shared_ptr<Font> font, std::shared_ptr<GameObject> gameObjrct)
	: TextComponent(text, font, gameObjrct)
{
}

void FpsComponent::Update(float deltaTime)
{
	TextComponent::Update(deltaTime);

	float fps{ 1.f / deltaTime };
	TextComponent::SetText(std::format("FPS: {:.1f}", fps));
}

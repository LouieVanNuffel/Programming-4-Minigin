#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "RenderComponent.h"
#include "FpsComponent.h"
#include "RotationComponent.h"
#include "ThrashTheCacheComponent.h"
#include "InputManager.h"
#include "MoveCommand.h"
#include "AddMoveDirectionCommand.h"
#include "Windows.h"
#include "Xinput.h"
#include "Scene.h"

using namespace dae;

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	//Background
	auto backgroundObject = std::make_shared<dae::GameObject>();
	auto textureComponent = std::make_unique<dae::RenderComponent>(backgroundObject.get());
	textureComponent->SetTexture("background.tga");
	backgroundObject->AddComponent(std::move(textureComponent));
	scene.Add(backgroundObject);

	//DAE Picture
	auto logoObject = std::make_shared<dae::GameObject>();
	textureComponent = std::make_unique<dae::RenderComponent>(logoObject.get());
	textureComponent->SetTexture("logo.tga");
	logoObject->SetPosition(320, 180);
	logoObject->AddComponent(std::move(textureComponent));
	scene.Add(logoObject);

	//Assignment Text
	auto textObject = std::make_shared<dae::GameObject>();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto textComponent = std::make_unique<dae::TextComponent>("Programming 4 Assignment", std::move(font), textObject.get());
	textObject->SetPosition(160, 2);
	textObject->AddComponent(std::move(textComponent));
	scene.Add(textObject);

	//FPS Counter
	auto fpsObject = std::make_shared<dae::GameObject>();
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto fpsTextComponent = std::make_unique<dae::TextComponent>("FPS: ", font, fpsObject.get());
	auto fpsComponent = std::make_unique<dae::FpsComponent>(fpsObject.get());
	fpsObject->SetPosition(2, 2);
	fpsObject->AddComponent(std::move(fpsTextComponent));
	fpsObject->AddComponent(std::move(fpsComponent));
	scene.Add(fpsObject);

	//Characters
	auto characterObject1 = std::make_shared<dae::GameObject>();
	textureComponent = std::make_unique<dae::RenderComponent>(characterObject1.get());
	textureComponent->SetTexture("PengoRed.png");
	auto moveComponent = std::make_unique<dae::MoveComponent>(characterObject1.get(), 50.f, true);
	characterObject1->AddComponent(std::move(textureComponent));
	characterObject1->AddComponent(std::move(moveComponent));
	characterObject1->SetPosition(180, 180);
	scene.Add(characterObject1);

	auto characterObject2 = std::make_shared<dae::GameObject>();
	textureComponent = std::make_unique<dae::RenderComponent>(characterObject2.get());
	textureComponent->SetTexture("PengoGreen.png");
	moveComponent = std::make_unique<dae::MoveComponent>(characterObject2.get(), 100.f);
	characterObject2->AddComponent(std::move(textureComponent));
	characterObject2->AddComponent(std::move(moveComponent));
	characterObject2->SetPosition(160, 180);
	scene.Add(characterObject2);

	////Thrash The Cache
	//auto thrashTheCacheObject = std::make_shared<dae::GameObject>();
	//auto thrashTheCacheComponent = std::make_unique<dae::ThrashTheCacheComponent>(thrashTheCacheObject.get());
	//thrashTheCacheObject->AddComponent(std::move(thrashTheCacheComponent));
	//scene.Add(thrashTheCacheObject);

	//Commands (input)
	auto addUpCommand = std::make_unique<AddMoveDirectionCommand>(characterObject1.get(), 1.f, 0.f, 0.f, 0.f);
	auto removeUpCommand = std::make_unique<AddMoveDirectionCommand>(characterObject1.get(), -1.f, 0.f, 0.f, 0.f);

	auto addDownCommand = std::make_unique<AddMoveDirectionCommand>(characterObject1.get(), 0.f, 1.f, 0.f, 0.f);
	auto removeDownCommand = std::make_unique<AddMoveDirectionCommand>(characterObject1.get(), 0.f, -1.f, 0.f, 0.f);

	auto addLeftCommand = std::make_unique<AddMoveDirectionCommand>(characterObject1.get(), 0.f, 0.f, 1.f, 0.f);
	auto removeLeftCommand = std::make_unique<AddMoveDirectionCommand>(characterObject1.get(), 0.f, 0.f, -1.f, 0.f);

	auto addRightCommand = std::make_unique<AddMoveDirectionCommand>(characterObject1.get(), 0.f, 0.f, 0.f, 1.f);
	auto removeRightCommand = std::make_unique<AddMoveDirectionCommand>(characterObject1.get(), 0.f, 0.f, 0.f, -1.f);

	InputManager::GetInstance().BindCommandToKeyboard(std::move(addUpCommand), SDLK_w, SDL_KEYDOWN);
	InputManager::GetInstance().BindCommandToKeyboard(std::move(addDownCommand), SDLK_s, SDL_KEYDOWN);
	InputManager::GetInstance().BindCommandToKeyboard(std::move(addLeftCommand), SDLK_a, SDL_KEYDOWN);
	InputManager::GetInstance().BindCommandToKeyboard(std::move(addRightCommand), SDLK_d, SDL_KEYDOWN);

	InputManager::GetInstance().BindCommandToKeyboard(std::move(removeUpCommand), SDLK_w, SDL_KEYUP);
	InputManager::GetInstance().BindCommandToKeyboard(std::move(removeDownCommand), SDLK_s, SDL_KEYUP);
	InputManager::GetInstance().BindCommandToKeyboard(std::move(removeLeftCommand), SDLK_a, SDL_KEYUP);
	InputManager::GetInstance().BindCommandToKeyboard(std::move(removeRightCommand), SDLK_d, SDL_KEYUP);

	auto moveUpCommand = std::make_unique<MoveCommand>(characterObject2.get(), 0.f, -1.f);
	auto moveDownCommand = std::make_unique<MoveCommand>(characterObject2.get(), 0.f, 1.f);
	auto moveLeftCommand = std::make_unique<MoveCommand>(characterObject2.get(), -1.f, .0f);
	auto moveRightCommand = std::make_unique<MoveCommand>(characterObject2.get(), 1.f, 0.f);

	InputManager::GetInstance().BindCommandToController(std::move(moveUpCommand), XINPUT_GAMEPAD_DPAD_UP, PollType::IsPressed);
	InputManager::GetInstance().BindCommandToController(std::move(moveDownCommand), XINPUT_GAMEPAD_DPAD_DOWN, PollType::IsPressed);
	InputManager::GetInstance().BindCommandToController(std::move(moveLeftCommand), XINPUT_GAMEPAD_DPAD_LEFT, PollType::IsPressed);
	InputManager::GetInstance().BindCommandToController(std::move(moveRightCommand), XINPUT_GAMEPAD_DPAD_RIGHT, PollType::IsPressed);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}
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
#include "LivesDisplayComponent.h"
#include "HealthComponent.h"
#include "TakeDamageCommand.h"
#include "PointComponent.h"
#include "PointsDisplayComponent.h"
#include "PickUpPointCommand.h"
#include "Controller.h"
#include "Subject.h"
#include "Scene.h"
#include "ServiceLocator.h"
#include "QueuedSoundSystem.h"
#include "LoggingSoundSystem.h"
#include "PlaySoundCommand.h"
#include "Animator.h"
#include "VelocityComponent.h"
#include "Level.h"
#include "PengoCharacter.h"
#include "CollisionSystem.h"
#include "PushCommand.h"
#include "SnoBeeCharacter.h"
#include "LevelState.h"
#include "LevelTimerComponent.h"

using namespace dae;

void Load()
{
#if _DEBUG
	ServiceLocator::register_sound_system(std::make_unique<LoggingSoundSystem>(std::make_unique<QueuedSoundSystem>()));
#else
	ServiceLocator::register_sound_system(std::make_unique<QueuedSoundSystem>());
#endif
	auto& ss = ServiceLocator::get_sound_system();
	ss.AddAudioClip(0, "../Data/CreditSound.wav");

	auto& scene = dae::SceneManager::GetInstance().CreateScene("Level1");

	//Level
	Level level{ "Level1", 16, 1.5f, 150, 50 };
	const std::vector<std::shared_ptr<GameObject>>& levelGameObjects = level.LoadLevelGameObjects();
	for (uint32_t index{}; index < levelGameObjects.size(); ++index)
	{
		scene.Add(levelGameObjects[index]);
	}

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

	//Level Timer
	auto timerObject = std::make_shared<dae::GameObject>();
	auto levelTimerComponent = std::make_unique<LevelTimerComponent>(timerObject.get());
	timerObject->AddComponent(std::move(levelTimerComponent));
	scene.Add(timerObject);

	//Characters
	PengoCharacter pengo1{ PengoColor::red };
	auto characterObject1 = pengo1.GetCharacterObject();
	characterObject1->SetPosition(200.0f, 180.0f);
	scene.Add(characterObject1);
	LevelState::GetInstance().AddPlayerObject(characterObject1.get());

	PengoCharacter pengo2{ PengoColor::green };
	auto characterObject2 = pengo2.GetCharacterObject();
	characterObject2->SetPosition(160.0f, 180.0f);
	scene.Add(characterObject2);
	LevelState::GetInstance().AddPlayerObject(characterObject2.get());

	// Display Controls
	auto controllerControlsTextObject = std::make_shared<dae::GameObject>();
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);
	textComponent = std::make_unique<dae::TextComponent>("Use the D-Pad to move green pengo, X to inflict damage, A to pick up points (sound)", std::move(font), controllerControlsTextObject.get());
	controllerControlsTextObject->SetPosition(0, 50);
	controllerControlsTextObject->AddComponent(std::move(textComponent));
	scene.Add(controllerControlsTextObject);

	auto keyboardControlsTextObject = std::make_shared<dae::GameObject>();
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);
	textComponent = std::make_unique<dae::TextComponent>("Use WASD to move red pengo, C to inflict damage, Z to pick up points (sound)", std::move(font), keyboardControlsTextObject.get());
	keyboardControlsTextObject->SetPosition(0, 70);
	keyboardControlsTextObject->AddComponent(std::move(textComponent));
	scene.Add(keyboardControlsTextObject);

	// Display Lives
	// Character 1
	auto livesDisplayObject1 = std::make_shared<GameObject>();
	livesDisplayObject1->SetPosition(0, 100);
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	auto livesDisplayComponent = std::make_unique<LivesDisplayComponent>("# lives: " + std::to_string(characterObject1->GetComponent<HealthComponent>()->GetLives()),
		std::move(font), livesDisplayObject1.get());

	characterObject1->GetComponent<Subject>()->AddObserver(livesDisplayComponent.get());

	livesDisplayObject1->AddComponent(std::move(livesDisplayComponent));
	scene.Add(livesDisplayObject1);

	// Character 2
	auto livesDisplayObject2 = std::make_shared<GameObject>();
	livesDisplayObject2->SetPosition(0, 140);
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	livesDisplayComponent = std::make_unique<LivesDisplayComponent>("# lives: " + std::to_string(characterObject2->GetComponent<HealthComponent>()->GetLives()),
		std::move(font), livesDisplayObject2.get());

	characterObject2->GetComponent<Subject>()->AddObserver(livesDisplayComponent.get());

	livesDisplayObject2->AddComponent(std::move(livesDisplayComponent));
	scene.Add(livesDisplayObject2);

	// Display Points
	// Character 1
	auto pointsDisplayObject1 = std::make_shared<GameObject>();
	pointsDisplayObject1->SetPosition(0, 120);
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	auto pointsDisplayComponent = std::make_unique<PointsDisplayComponent>("Score: " + std::to_string(characterObject1->GetComponent<PointComponent>()->GetPoints()),
		std::move(font), pointsDisplayObject1.get());

	characterObject1->GetComponent<Subject>()->AddObserver(pointsDisplayComponent.get());

	pointsDisplayObject1->AddComponent(std::move(pointsDisplayComponent));
	scene.Add(pointsDisplayObject1);

	// Character 2
	auto pointsDisplayObject2 = std::make_shared<GameObject>();
	pointsDisplayObject2->SetPosition(0, 160);
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	pointsDisplayComponent = std::make_unique<PointsDisplayComponent>("Score: " + std::to_string(characterObject1->GetComponent<PointComponent>()->GetPoints()),
		std::move(font), pointsDisplayObject2.get());

	characterObject2->GetComponent<Subject>()->AddObserver(pointsDisplayComponent.get());

	pointsDisplayObject2->AddComponent(std::move(pointsDisplayComponent));
	scene.Add(pointsDisplayObject2);


	////Thrash The Cache
	//auto thrashTheCacheObject = std::make_shared<dae::GameObject>();
	//auto thrashTheCacheComponent = std::make_unique<dae::ThrashTheCacheComponent>(thrashTheCacheObject.get());
	//thrashTheCacheObject->AddComponent(std::move(thrashTheCacheComponent));
	//scene.Add(thrashTheCacheObject);

	//Commands (input)
	auto moveUpCommand = std::make_unique<MoveCommand>(characterObject1.get(), 0.f, -1.f, 50.f);
	auto moveDownCommand = std::make_unique<MoveCommand>(characterObject1.get(), 0.f, 1.f, 50.f);
	auto moveLeftCommand = std::make_unique<MoveCommand>(characterObject1.get(), -1.f, .0f, 50.f);
	auto moveRightCommand = std::make_unique<MoveCommand>(characterObject1.get(), 1.f, 0.f, 50.f);
	auto inflictDamage = std::make_unique<TakeDamageCommand>(characterObject1.get(), 100.f);
	auto pickUpPoint = std::make_unique<PickUpPointCommand>(characterObject1.get(), 1);
	auto pushCommand = std::make_unique<PushCommand>(characterObject1.get());

	//Commands (sound)
	auto creditSoundCommand = std::make_unique<PlaySoundCommand>(SoundId{ 0 }, 100.0f);

	InputManager::GetInstance().BindCommandToKeyboard(std::move(moveUpCommand), SDL_SCANCODE_W);
	InputManager::GetInstance().BindCommandToKeyboard(std::move(moveDownCommand), SDL_SCANCODE_S);
	InputManager::GetInstance().BindCommandToKeyboard(std::move(moveLeftCommand), SDL_SCANCODE_A);
	InputManager::GetInstance().BindCommandToKeyboard(std::move(moveRightCommand), SDL_SCANCODE_D);
	InputManager::GetInstance().BindCommandToKeyboard(std::move(inflictDamage), SDL_SCANCODE_C);
	InputManager::GetInstance().BindCommandToKeyboard(std::move(pickUpPoint), SDL_SCANCODE_Z);
	InputManager::GetInstance().BindCommandToKeyboard(std::move(creditSoundCommand), SDL_SCANCODE_Z);
	InputManager::GetInstance().BindCommandToKeyboard(std::move(pushCommand), SDL_SCANCODE_SPACE);

	moveUpCommand = std::make_unique<MoveCommand>(characterObject2.get(), 0.f, -1.f, 100.f);
	moveDownCommand = std::make_unique<MoveCommand>(characterObject2.get(), 0.f, 1.f, 100.f);
	moveLeftCommand = std::make_unique<MoveCommand>(characterObject2.get(), -1.f, .0f, 100.f);
	moveRightCommand = std::make_unique<MoveCommand>(characterObject2.get(), 1.f, 0.f, 100.f);
	inflictDamage = std::make_unique<TakeDamageCommand>(characterObject2.get(), 100.f);
	pickUpPoint = std::make_unique<PickUpPointCommand>(characterObject2.get(), 1);
	pushCommand = std::make_unique<PushCommand>(characterObject2.get());

	creditSoundCommand = std::make_unique<PlaySoundCommand>(SoundId{0}, 100.0f);

	InputManager::GetInstance().BindCommandToController(std::move(moveUpCommand), XINPUT_GAMEPAD_DPAD_UP, PollType::IsPressed, 0);
	InputManager::GetInstance().BindCommandToController(std::move(moveDownCommand), XINPUT_GAMEPAD_DPAD_DOWN, PollType::IsPressed, 0);
	InputManager::GetInstance().BindCommandToController(std::move(moveLeftCommand), XINPUT_GAMEPAD_DPAD_LEFT, PollType::IsPressed, 0);
	InputManager::GetInstance().BindCommandToController(std::move(moveRightCommand), XINPUT_GAMEPAD_DPAD_RIGHT, PollType::IsPressed, 0);
	InputManager::GetInstance().BindCommandToController(std::move(inflictDamage), XINPUT_GAMEPAD_X, PollType::IsPressed, 0);
	InputManager::GetInstance().BindCommandToController(std::move(pickUpPoint), XINPUT_GAMEPAD_A, PollType::IsPressed, 0);
	InputManager::GetInstance().BindCommandToController(std::move(creditSoundCommand), XINPUT_GAMEPAD_A, PollType::IsPressed, 0);
	InputManager::GetInstance().BindCommandToController(std::move(pushCommand), XINPUT_GAMEPAD_B, PollType::IsPressed, 0);
}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(Load);
	return 0;
}

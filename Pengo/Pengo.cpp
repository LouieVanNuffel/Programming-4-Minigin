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
#include "KeyboardControllerComponent.h"
#include "GamepadControllerComponent.h"
#include "Actions.h"
#include "TilebasedMoveCommand.h"
#include "NextSceneCommand.h"
#include "SwitchPlayerCountCommand.h"
#include "DisplayPlayerCountComponent.h"
#include "InactiveWhenSoloPlayerComponent.h"

using namespace dae;

void LoadLevelScene(const std::string levelName)
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene(levelName);
	dae::SceneManager::GetInstance().SetActiveScene(levelName);

	//Level State
	auto levelStateObject = std::make_shared<GameObject>();
	auto levelState = std::make_unique<LevelState>(levelStateObject.get());
	scene.levelState = levelState.get();
	levelStateObject->AddComponent(std::move(levelState));
	scene.Add(levelStateObject);

	//Level
	Level level{ levelName, 16, 1.5f, 150, 50 };
	const std::vector<std::shared_ptr<GameObject>>& levelGameObjects = level.LoadLevelGameObjects();
	for (uint32_t index{}; index < levelGameObjects.size(); ++index)
	{
		scene.Add(levelGameObjects[index]);
	}

	//FPS Counter
	auto fpsObject = std::make_shared<dae::GameObject>();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
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
	glm::vec3 spawnPosition = scene.levelState->GetPlayerSpawnPosition();
	characterObject1->SetPosition(spawnPosition.x, spawnPosition.y);
	scene.Add(characterObject1);

	PengoCharacter pengo2{ PengoColor::green };
	auto characterObject2 = pengo2.GetCharacterObject();
	auto inactiveWhenSoloPlayerComponent = std::make_unique<InactiveWhenSoloPlayerComponent>(characterObject2.get());
	characterObject2->AddComponent(std::move(inactiveWhenSoloPlayerComponent));
	characterObject2->SetPosition(spawnPosition.x, spawnPosition.y);
	scene.Add(characterObject2);

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
	inactiveWhenSoloPlayerComponent = std::make_unique<InactiveWhenSoloPlayerComponent>(livesDisplayObject2.get());
	livesDisplayObject2->AddComponent(std::move(inactiveWhenSoloPlayerComponent));
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
	inactiveWhenSoloPlayerComponent = std::make_unique<InactiveWhenSoloPlayerComponent>(pointsDisplayObject2.get());
	pointsDisplayObject2->AddComponent(std::move(inactiveWhenSoloPlayerComponent));
	scene.Add(pointsDisplayObject2);

	//Commands (input)
	auto moveUpCommand = std::make_unique<TileBasedMoveCommand>(characterObject1.get(), 0.f, -1.f);
	auto moveDownCommand = std::make_unique<TileBasedMoveCommand>(characterObject1.get(), 0.f, 1.f);
	auto moveLeftCommand = std::make_unique<TileBasedMoveCommand>(characterObject1.get(), -1.f, .0f);
	auto moveRightCommand = std::make_unique<TileBasedMoveCommand>(characterObject1.get(), 1.f, 0.f);
	auto pushCommand = std::make_unique<PushCommand>(characterObject1.get());

	auto keyboardControllerComponent = std::make_unique<KeyboardControllerComponent>(characterObject1.get());
	keyboardControllerComponent->BindCommandToAction(std::move(moveUpCommand), static_cast<uint32_t>(Action::up));
	keyboardControllerComponent->BindCommandToAction(std::move(moveDownCommand), static_cast<uint32_t>(Action::down));
	keyboardControllerComponent->BindCommandToAction(std::move(moveLeftCommand), static_cast<uint32_t>(Action::left));
	keyboardControllerComponent->BindCommandToAction(std::move(moveRightCommand), static_cast<uint32_t>(Action::right));
	keyboardControllerComponent->BindCommandToAction(std::move(pushCommand), static_cast<uint32_t>(Action::push));
	characterObject1->AddComponent(std::move(keyboardControllerComponent));

	moveUpCommand = std::make_unique<TileBasedMoveCommand>(characterObject1.get(), 0.f, -1.f);
	moveDownCommand = std::make_unique<TileBasedMoveCommand>(characterObject1.get(), 0.f, 1.f);
	moveLeftCommand = std::make_unique<TileBasedMoveCommand>(characterObject1.get(), -1.f, .0f);
	moveRightCommand = std::make_unique<TileBasedMoveCommand>(characterObject1.get(), 1.f, 0.f);
	pushCommand = std::make_unique<PushCommand>(characterObject1.get());

	auto gamepadControllerComponent = std::make_unique<GamepadControllerComponent>(characterObject1.get(), 1);
	gamepadControllerComponent->BindCommandToAction(std::move(moveUpCommand), static_cast<uint32_t>(Action::up));
	gamepadControllerComponent->BindCommandToAction(std::move(moveDownCommand), static_cast<uint32_t>(Action::down));
	gamepadControllerComponent->BindCommandToAction(std::move(moveLeftCommand), static_cast<uint32_t>(Action::left));
	gamepadControllerComponent->BindCommandToAction(std::move(moveRightCommand), static_cast<uint32_t>(Action::right));
	gamepadControllerComponent->BindCommandToAction(std::move(pushCommand), static_cast<uint32_t>(Action::push));
	characterObject1->AddComponent(std::move(gamepadControllerComponent));

	moveUpCommand = std::make_unique<TileBasedMoveCommand>(characterObject2.get(), 0.f, -1.f);
	moveDownCommand = std::make_unique<TileBasedMoveCommand>(characterObject2.get(), 0.f, 1.f);
	moveLeftCommand = std::make_unique<TileBasedMoveCommand>(characterObject2.get(), -1.f, .0f);
	moveRightCommand = std::make_unique<TileBasedMoveCommand>(characterObject2.get(), 1.f, 0.f);
	pushCommand = std::make_unique<PushCommand>(characterObject2.get());

	gamepadControllerComponent = std::make_unique<GamepadControllerComponent>(characterObject2.get(), 0);
	gamepadControllerComponent->BindCommandToAction(std::move(moveUpCommand), static_cast<uint32_t>(Action::up));
	gamepadControllerComponent->BindCommandToAction(std::move(moveDownCommand), static_cast<uint32_t>(Action::down));
	gamepadControllerComponent->BindCommandToAction(std::move(moveLeftCommand), static_cast<uint32_t>(Action::left));
	gamepadControllerComponent->BindCommandToAction(std::move(moveRightCommand), static_cast<uint32_t>(Action::right));
	gamepadControllerComponent->BindCommandToAction(std::move(pushCommand), static_cast<uint32_t>(Action::push));
	characterObject1->AddComponent(std::move(gamepadControllerComponent));

	auto generalControlObject = std::make_shared<dae::GameObject>();
	keyboardControllerComponent = std::make_unique<KeyboardControllerComponent>(generalControlObject.get());
	auto nextSceneCommand = std::make_unique<NextSceneCommand>();
	keyboardControllerComponent->BindCommandToAction(std::move(nextSceneCommand), static_cast<uint32_t>(Action::nextScene));
	generalControlObject->AddComponent(std::move(keyboardControllerComponent));
	scene.Add(generalControlObject);
}

void LoadStartScene()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("start");
	dae::SceneManager::GetInstance().SetActiveScene("start");

	//Level State
	auto levelStateObject = std::make_shared<GameObject>();
	auto levelState = std::make_unique<LevelState>(levelStateObject.get());
	scene.levelState = levelState.get();
	levelStateObject->AddComponent(std::move(levelState));
	scene.Add(levelStateObject);

	// Display Title
	auto titleTextObject = std::make_shared<dae::GameObject>();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 32);
	auto textComponent = std::make_unique<dae::TextComponent>("PENGO", std::move(font), titleTextObject.get());
	titleTextObject->SetPosition(50, 50);
	titleTextObject->AddComponent(std::move(textComponent));
	scene.Add(titleTextObject);

	// Display Player Count
	auto playerCountTextObject = std::make_shared<dae::GameObject>();
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);
	textComponent = std::make_unique<DisplayPlayerCountComponent>("2 Players", std::move(font), playerCountTextObject.get());
	playerCountTextObject->SetPosition(50, 100);
	playerCountTextObject->AddComponent(std::move(textComponent));
	scene.Add(playerCountTextObject);

	// Display Start Key
	auto startKeyTextObject = std::make_shared<dae::GameObject>();
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);
	textComponent = std::make_unique<dae::TextComponent>("Press f1 to start", std::move(font), startKeyTextObject.get());
	startKeyTextObject->SetPosition(50, 125);
	startKeyTextObject->AddComponent(std::move(textComponent));
	scene.Add(startKeyTextObject);

	// Key bindings
	auto generalControlObject = std::make_shared<dae::GameObject>();
	auto keyboardControllerComponent = std::make_unique<KeyboardControllerComponent>(generalControlObject.get());
	auto nextSceneCommand = std::make_unique<NextSceneCommand>();
	auto switchPlayerCountCommand = std::make_unique<SwitchPlayerCountCommand>();
	keyboardControllerComponent->BindCommandToAction(std::move(nextSceneCommand), static_cast<uint32_t>(Action::nextScene));
	keyboardControllerComponent->BindCommandToAction(std::move(switchPlayerCountCommand), static_cast<uint32_t>(Action::switchPlayerCount));
	generalControlObject->AddComponent(std::move(keyboardControllerComponent));
	scene.Add(generalControlObject);
}

void Load()
{
#if _DEBUG
	ServiceLocator::register_sound_system(std::make_unique<LoggingSoundSystem>(std::make_unique<QueuedSoundSystem>()));
#else
	ServiceLocator::register_sound_system(std::make_unique<QueuedSoundSystem>());
#endif
	auto& ss = ServiceLocator::get_sound_system();
	ss.AddAudioClip(0, "../Data/CreditSound.wav");

	LoadStartScene();
	LoadLevelScene("Level1");
	LoadLevelScene("Level2");
	LoadLevelScene("Level3");
	SceneManager::GetInstance().SetActiveScene(0);
}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(Load);
	return 0;
}

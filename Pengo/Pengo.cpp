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
	Level level{ "Level3", 16, 1.5f, 150, 50 };
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
	glm::vec3 spawnPosition = LevelState::GetInstance().GetPlayerSpawnPosition();
	characterObject1->SetPosition(spawnPosition.x, spawnPosition.y);
	scene.Add(characterObject1);

	PengoCharacter pengo2{ PengoColor::green };
	auto characterObject2 = pengo2.GetCharacterObject();
	characterObject2->SetPosition(spawnPosition.x, spawnPosition.y);
	scene.Add(characterObject2);

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

	//Commands (input)

	auto moveUpCommand = std::make_unique<MoveCommand>(characterObject1.get(), 0.f, -1.f, 50.f);
	auto moveDownCommand = std::make_unique<MoveCommand>(characterObject1.get(), 0.f, 1.f, 50.f);
	auto moveLeftCommand = std::make_unique<MoveCommand>(characterObject1.get(), -1.f, .0f, 50.f);
	auto moveRightCommand = std::make_unique<MoveCommand>(characterObject1.get(), 1.f, 0.f, 50.f);
	auto pushCommand = std::make_unique<PushCommand>(characterObject1.get());

	auto keyboardControllerComponent = std::make_unique<KeyboardControllerComponent>(characterObject1.get());
	keyboardControllerComponent->BindCommandToAction(std::move(moveUpCommand), static_cast<uint32_t>(Action::up));
	keyboardControllerComponent->BindCommandToAction(std::move(moveDownCommand), static_cast<uint32_t>(Action::down));
	keyboardControllerComponent->BindCommandToAction(std::move(moveLeftCommand), static_cast<uint32_t>(Action::left));
	keyboardControllerComponent->BindCommandToAction(std::move(moveRightCommand), static_cast<uint32_t>(Action::right));
	keyboardControllerComponent->BindCommandToAction(std::move(pushCommand), static_cast<uint32_t>(Action::push));
	characterObject1->AddComponent(std::move(keyboardControllerComponent));

	moveUpCommand = std::make_unique<MoveCommand>(characterObject2.get(), 0.f, -1.f, 100.f);
	moveDownCommand = std::make_unique<MoveCommand>(characterObject2.get(), 0.f, 1.f, 100.f);
	moveLeftCommand = std::make_unique<MoveCommand>(characterObject2.get(), -1.f, .0f, 100.f);
	moveRightCommand = std::make_unique<MoveCommand>(characterObject2.get(), 1.f, 0.f, 100.f);
	pushCommand = std::make_unique<PushCommand>(characterObject2.get());

	auto gamepadControllerComponent = std::make_unique<GamepadControllerComponent>(characterObject2.get(), 0);
	gamepadControllerComponent->BindCommandToAction(std::move(moveUpCommand), static_cast<uint32_t>(Action::up));
	gamepadControllerComponent->BindCommandToAction(std::move(moveDownCommand), static_cast<uint32_t>(Action::down));
	gamepadControllerComponent->BindCommandToAction(std::move(moveLeftCommand), static_cast<uint32_t>(Action::left));
	gamepadControllerComponent->BindCommandToAction(std::move(moveRightCommand), static_cast<uint32_t>(Action::right));
	gamepadControllerComponent->BindCommandToAction(std::move(pushCommand), static_cast<uint32_t>(Action::push));
	characterObject1->AddComponent(std::move(gamepadControllerComponent));
}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(Load);
	return 0;
}

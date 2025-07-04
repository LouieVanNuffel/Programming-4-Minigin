#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <stdexcept>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "EngineTime.h"
#include "CollisionSystem.h"

#include <thread>

SDL_Window* g_window{};

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

dae::Minigin::Minigin(const std::string &dataPath)
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		throw std::runtime_error(std::string("Mix_OpenAudio Error: ") + Mix_GetError());
	}

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(g_window);

	ResourceManager::GetInstance().Init(dataPath);

	Time::GetInstance().SetFixedTime(m_FixedTimeStep);
}

dae::Minigin::~Minigin()
{
	Renderer::GetInstance().Destroy();
	Mix_CloseAudio();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run(const std::function<void()>& Load)
{
	Load();

	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();
	auto& collisionSystem = CollisionSystem::GetInstance();

	if (!m_HasStarted) sceneManager.Start(), m_HasStarted = true; // Call start

	bool doContinue = true;
	auto last_time = std::chrono::high_resolution_clock::now();
	float lag = 0.0f;
	while (doContinue)
	{
		const auto current_time = std::chrono::high_resolution_clock::now();
		const float delta_time = std::chrono::duration<float>(current_time - last_time).count();
		Time::GetInstance().SetDeltaTime(delta_time);
		last_time = current_time;
		lag += delta_time;

		doContinue = input.ProcessInput();
		while (lag >= m_FixedTimeStep)
		{
			sceneManager.FixedUpdate();
			lag -= m_FixedTimeStep;
		}
		sceneManager.Update();
		collisionSystem.Update();
		sceneManager.LateUpdate();
		renderer.Render();
		sceneManager.CleanUpDestroyed();
		const auto sleep_time = current_time + std::chrono::milliseconds(m_MillisecondsPerFrame) - 
																		std::chrono::high_resolution_clock::now();
		std::this_thread::sleep_for(sleep_time);
	}

}

#include "Engine.h"

Engine::Mutex mutexManagerLock;

void Engine::Init()
{
	Renderer::Initialize();
	Physics::Initialize();
	//Get Input from user
	Input::CheckForInput();
}
void Engine::Run()
{
	bool quitRequested = false;
	Timing t_Instance;
	float dt = t_Instance.CalcLastFrameTime_ms();

	GLib::Service(quitRequested);
 	
	quitRequested = Input::QuitRequested();

	Renderer::Draw();

	Physics::Run(dt);

	Collision::Run(dt);
}

void Engine::Shutdown()
{
	Renderer::Shutdown();
	Physics::Shutdown();
	Collision::Shutdown();
	Manager::Shutdown();
	GLib::Shutdown();
 	
}



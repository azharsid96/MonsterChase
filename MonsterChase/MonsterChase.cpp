#include "pch.h"
#include "Core/Engine.h"
#include "Core/LoadGameData.h"

#if defined _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif // _DEBUG

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <vector>
#include <functional>
#include <Windows.h>

float RandomFloat(float min, float max) {

	assert(max > min);
	
	float random = (static_cast<float>(rand())) / static_cast<float>(RAND_MAX);
	float diff = max - min;
	float r = random * diff;
	return min + r;
}

static void ProcessFileContents(std::vector<uint8_t> i_Contents, std::function<void(Json&)> i_Processor, Engine::Event* i_pFinishEvent = nullptr)
{
	if (!i_Contents.empty())
	{
		if (!Engine::JobSystem::ShutdownRequested())
		{
			Json data = Json::parse(i_Contents);
			i_Processor(data);
		}
	}

	if (i_pFinishEvent)
		i_pFinishEvent->Signal();
}

void ProcessFile(const char* i_Filename, std::function<void(Json&)> i_Processor, Engine::Event* i_pFinishEvent, const Engine::HashedString& i_JobQueue)
{
	Engine::JobSystem::RunJob("ProcessFile", [i_JobQueue, i_Filename, i_Processor, i_pFinishEvent]()
	{
		std::vector<uint8_t> Contents = Helpers::LoadFileToBuffer(i_Filename);

		if (!Contents.empty())
		{
			if (!Engine::JobSystem::ShutdownRequested())
			{
				// this works around C++11 issue with capturing member variable by value
				std::function<void(Json&)> Processor = i_Processor;
				Engine::Event* pFinishEvent = i_pFinishEvent;

				Engine::JobSystem::RunJob("ProcessFileContents", [Contents, Processor, pFinishEvent]()
				{
					ProcessFileContents(Contents, Processor, pFinishEvent);
				}
				, i_JobQueue);
			}
		} }
	, i_JobQueue);
}

void CreateGameObjectAsync(const char * i_filename)
{
	using namespace std::placeholders;
	Engine::JobSystem::CreateQueue("Default", 2);
	ProcessFile(i_filename, std::bind(Engine::CreateObjectFromJson, _1), nullptr, "Default");
}

void CheckMonsterBounds(WeakPtrs<GameObject> monsterObj)
{
	using namespace Engine::Math;

	std::string monsterName = monsterObj.Acquire()->GetName();
	
	float monsterXPos = monsterObj.Acquire()->GetPosition().x();
	float monsterYPos = monsterObj.Acquire()->GetPosition().y();

	float leftBounds = -600.0f;
	float rightBounds = 600.0f;
	float bottomBounds = -450.0f;
	float topBounds = 250.0f;

	float maxForceX = 200.0f;
	float minForceX = -200.0f;
	float maxForceY = 100.0f;
	float minForceY = -100.0f;

	float monsterWidth = monsterObj.Acquire()->GetExtents().x() * 2.0f;
	float monsterHeight = monsterObj.Acquire()->GetExtents().y() * 2.0f;


	if (monsterXPos <= leftBounds)
	{
		// Move monster right
		Physics::UpdateForce(Vector3(RandomFloat(0.0f, maxForceX), RandomFloat(minForceY, maxForceY), 0.0f), monsterName);
	}
	if (monsterXPos >= rightBounds)
	{
		// // Move monster left
		Physics::UpdateForce(Vector3(RandomFloat(minForceX, 0.0f), RandomFloat(minForceY, maxForceY), 0.0f), monsterName);
	}
	if (monsterYPos >= topBounds)
	{
		// Move monster down
		Physics::UpdateForce(Vector3(RandomFloat(minForceX, maxForceX), RandomFloat(minForceY, 0.0f), 0.0f), monsterName);
	}
	if (monsterYPos <= bottomBounds)
	{
		// Move monster up
		Physics::UpdateForce(Vector3(RandomFloat(minForceX, maxForceX), RandomFloat(0.0f, maxForceY), 0.0f), monsterName);
	}

	if ((monsterYPos - topBounds >= 500.0f) || (monsterYPos - bottomBounds <= -500.0f) || (monsterXPos - rightBounds >= 500.0f) || 
		(monsterXPos - leftBounds <= -500.0f))
	{
		//monsterObj.Acquire()->SetVelocity(Vector3::Zero);
		monsterObj.Acquire()->SetPosition(Vector3(0.0f, topBounds + monsterHeight, 0.0f));
	}
}

void CheckBoxBounds(WeakPtrs<GameObject> boxObj)
{
	using namespace Engine::Math;

	std::string boxName = boxObj.Acquire()->GetName();

	float boxXPos = boxObj.Acquire()->GetPosition().x();
	float boxYPos = boxObj.Acquire()->GetPosition().y();

	float leftBounds = -700.0f;
	float rightBounds = 700.0f;
	float bottomBounds = -450.0f;
	float topBounds = 350.0f;

	float boxWidth = boxObj.Acquire()->GetExtents().x() * 2.0f;
	float boxHeight = boxObj.Acquire()->GetExtents().y() * 2.0f;

	if (boxXPos >= rightBounds + boxWidth)
	{
		boxObj.Acquire()->SetPosition(Vector3(leftBounds, boxObj.Acquire()->GetPosition().y(), boxObj.Acquire()->GetPosition().z()));
	}
	if (boxXPos <= leftBounds - boxWidth)
	{
		boxObj.Acquire()->SetPosition(Vector3(rightBounds, boxObj.Acquire()->GetPosition().y(), boxObj.Acquire()->GetPosition().z()));
	}
	if (boxYPos >= topBounds + boxHeight)
	{
		boxObj.Acquire()->SetPosition(Vector3(boxObj.Acquire()->GetPosition().x(), bottomBounds, boxObj.Acquire()->GetPosition().z()));
	}
	if (boxYPos <= bottomBounds - boxHeight)
	{
		boxObj.Acquire()->SetPosition(Vector3(boxObj.Acquire()->GetPosition().x(), topBounds, boxObj.Acquire()->GetPosition().z()));
	}
}

void MovePlayer(std::string playerName)
{
	using namespace Engine::Math;

	//Move the player
	if (Input::GetKeyPressed() == KC_D && Input::GetIsKeyDown())
	{
		Physics::UpdateForce(Vector3(1500.0f, 0.0f, 0.0f), playerName);
	}
	else if (Input::GetKeyPressed() == KC_A && Input::GetIsKeyDown())
	{

		Physics::UpdateForce(Vector3(-1500.0f, 0.0f, 0.0f), playerName);
	}
	else if (Input::GetKeyPressed() == KC_W && Input::GetIsKeyDown())
	{

		Physics::UpdateForce(Vector3(0.0f, 100.0f, 0.0f), playerName);
	}
	else if (Input::GetKeyPressed() == KC_S && Input::GetIsKeyDown())
	{

		Physics::UpdateForce(Vector3(0.0f, -100.0f, 0.0f), playerName);
	}
	else
	{
		Physics::UpdateForce(Vector3::Zero, playerName);
	}
}

void MoveMonster(WeakPtrs<GameObject> monsterObj)
{
	using namespace Engine::Math;

	std::string monsterName = monsterObj.Acquire()->GetName();
	
	Vector3 monsterPos = monsterObj.Acquire()->GetPosition();

	float maxForceX = 200.0f;
	float minForceX = -200.0f;
	float maxForceY = 100.0f;
	float minForceY = -100.0f;
	
	if (monsterObj.Acquire()->GetVelocity() == Vector3::Zero)
	{
		Physics::UpdateForce(Vector3(RandomFloat(minForceX, maxForceX), RandomFloat(minForceY, maxForceY), 0.0f), monsterName);
	}

	CheckMonsterBounds(monsterObj);
}


int WINAPI wWinMain(HINSTANCE i_hInstance, HINSTANCE i_hPrevInstance, LPWSTR i_lpCmdLine, int i_nCmdShow)
{	
	//Matrix_UnitTest();

	//_CrtSetBreakAlloc(742);
	
	bool bSuccess = GLib::Initialize(i_hInstance, i_nCmdShow, "GLibTest", -1, 1500, 900);
	Engine::Init();
	
	CreateGameObjectAsync("data/GameData.json");

	WeakPtrs<GameObject> player;
	WeakPtrs<GameObject> box;
	WeakPtrs<GameObject> goal;
	WeakPtrs<GameObject> monster1;
	WeakPtrs<GameObject> monster2;

	Engine::Math::Vector3 lastPlayerVelocity;
	
	while(!Input::QuitRequested() && !Collision::Death())
	{
		Manager::MoveGameObjects();
		
		Engine::Run();
		
		if (player && box && goal && monster1 && monster2)
		{
			// Moves the player around using key input
			MovePlayer(player.Acquire()->GetName());

			MoveMonster(monster1);
			MoveMonster(monster2);

			CheckBoxBounds(box);

			// Win condition if box gets "close" to goal
			if ((fabs(box.Acquire()->GetPosition().x() - goal.Acquire()->GetPosition().x()) <= 50.0f) && 
				(fabs(box.Acquire()->GetPosition().y() - goal.Acquire()->GetPosition().y()) <= 50.0f))
			{
				break;
			}

		}					
		else
		{
			player = WeakPtrs<GameObject>(Manager::GetObjectFromName("Player"));
			
			box = WeakPtrs<GameObject>(Manager::GetObjectFromName("Box"));

			goal = WeakPtrs<GameObject>(Manager::GetObjectFromName("Goal"));

			monster1 = WeakPtrs<GameObject>(Manager::GetObjectFromName("Monster 1"));
			
			monster2 = WeakPtrs<GameObject>(Manager::GetObjectFromName("Monster 2"));
		}
	}
	
	Engine::Shutdown();
	
#if defined _DEBUG
	_CrtDumpMemoryLeaks();
#endif // _DEBUG

}






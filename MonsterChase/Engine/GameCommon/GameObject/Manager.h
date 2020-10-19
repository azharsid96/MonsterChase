#pragma once
#include <vector>
#include "GameCommon/SmartPtrs/SmartPtrs.h"
#include "GameObject.h"
#include "Syncronization/Mutex.h"
#include "Syncronization/ScopeLock.h"

extern Engine::Mutex mutexManagerLock;

class Manager
{

public:
	inline static void Initialize() { };

	static Manager& Get()
	{
		static Manager* Instance = new Manager();
		return *Instance;
	}
	static void AddGameObject(GameObject* i_game_object)
	{
		Engine::ScopeLock scopeLock(mutexManagerLock);
		
		Get().m_newGameObjects.emplace_back(i_game_object);
		
	}

	static std::vector<SmartPtrs<GameObject>> & GetNewGameObjects()
	{
		return Get().m_newGameObjects;
	}

	static void MoveGameObjects()
	{
		Engine::ScopeLock scopeLock(mutexManagerLock);
		
		for (int i = 0; i < Get().m_newGameObjects.size(); i++)
		{
			Get().m_GameObjects.push_back(Get().m_newGameObjects[i]);
		}
		Get().m_newGameObjects.clear();

	}
	

	static SmartPtrs<GameObject> GetObjectFromName(const char * i_name)
	{
		
		Manager & obj = Get();
		for (int i = 0; i < obj.m_GameObjects.size(); i++)
		{
			if (obj.m_GameObjects[i])
			{
				if (obj.m_GameObjects[i]->GetName().compare(i_name) == 0)
				{
					return obj.m_GameObjects[i];
				}
			}
			
		}
		return SmartPtrs<GameObject>(nullptr);
	}

	inline static void Shutdown() { delete &Get(); };
private:
	std::vector<SmartPtrs<GameObject>> m_GameObjects;
	std::vector<SmartPtrs<GameObject>> m_newGameObjects;
};

#pragma once
#include "nlohmann/json.hpp"
#include "GameCommon/GameObject/Manager.h"
#include "Math/Vector2/Point2D.h"
#include "Math/Vector3/Vector3.h"
#include "Renderer/Renderer.h"
#include "Physics/Physics.h"
#include "Collision/Collision.h"
#include <string>
using Json = nlohmann::json;
namespace Engine
{
	inline void CreateObjectFromJson(Json& i_JsonGameData)
	{
		for(int i=0; i < i_JsonGameData["GameObjects"].size(); ++i)
		{
			Json GameObjectData = i_JsonGameData["GameObjects"][i];
			
			float x = GameObjectData["Position"]["x"];
			float y = GameObjectData["Position"]["y"];
			float z = GameObjectData["Position"]["z"];

			float z_Rotation = GameObjectData["Rotation"];
			
			Manager::AddGameObject(new GameObject(Math::Vector3(x, y, z), GameObjectData["Name"], z_Rotation));

			SmartPtrs<GameObject> GameObjData = Manager::GetNewGameObjects().back();
			
			if(GameObjectData.contains("Renderer"))
			{
				std::string File = GameObjectData["Renderer"];
				Renderer::AddGameObjectToRenderer(GameObjData, File);
			}

			if (GameObjectData.contains("Physics"))
			{
				float x = GameObjectData["Physics"]["ExternalForce"]["x"];
				float y = GameObjectData["Physics"]["ExternalForce"]["y"];
				float z = GameObjectData["Physics"]["ExternalForce"]["z"];

				float mass = GameObjectData["Physics"]["mass"];

				float drag_coefficient = GameObjectData["Physics"]["drag"];
				Math::Vector3 force = Math::Vector3(x, y, z);
				
				Physics::AddGameObjectToPhysics(GameObjData, force, mass, drag_coefficient);
			}

			if (GameObjectData.contains("Collision"))
			{
				if (GameObjectData["Collision"] ==  "true") 
					Collision::AddGameObjectToCollision(GameObjData);
			}
		}
	}
}


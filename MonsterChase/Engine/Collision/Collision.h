#pragma once
#include "Math/Matrix4/Matrix4.h"
#include "GameCommon/GameObject/GameObject.h"
#include "GameCommon/WeakPtrs/WeakPtrs.h"
#include "GLib.h"
#include "Helpers/Helpers.h"
#include "Syncronization/Mutex.h"
#include "Syncronization/ScopeLock.h"
#include "Input/Input.h"

#include "assert.h"
#include "vector"
#include "array"

struct Collidable
{
	WeakPtrs<GameObject> obj;
	Engine::Math::Point2D BBCenter;
	Engine::Math::Point2D Extents;

	Collidable() {}
	
	Collidable(WeakPtrs<GameObject> i_game_object) :
		obj(i_game_object)
	{
		BBCenter = i_game_object.Acquire()->GetBBCenter();
		Extents = i_game_object.Acquire()->GetExtents();
	}
};

struct CollisionPair
{
	Collidable objA;
	Collidable objB;
	Engine::Math::Vector4 collisionNormal;
	float collisionTime;
};

class Collision
{
public:
	static void AddGameObjectToCollision(SmartPtrs<GameObject> i_game_object);

	static void MoveObjects();
	
	//may require delta time param
	static void Run(float i_dt);

	static bool CheckForCollision(Collidable i_Object, Collidable & i_Other, float i_dt, Engine::Math::Vector4 & i_collisionNormal, float & i_collisionTime);

	static void CollisionResponse(Engine::Math::Vector4 i_CollisionNormal, Collidable & i_A, Collidable & i_B, float i_CollisionTime);

	inline static bool Death() { return c_Instance->exitOnCollide; }

	inline static void Shutdown() { delete Collision::c_Instance; };

private:
	static Collision*			c_Instance;
	Collision() {}
	std::vector<Collidable>		objsToCollide;
	std::vector<Collidable>		newObjsToCollide;
	bool						exitOnCollide = false;
};

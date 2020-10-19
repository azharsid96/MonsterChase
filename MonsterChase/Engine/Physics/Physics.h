#pragma once
#include "GLib.h"
#include "GameCommon/GameObject/GameObject.h"
#include "../GameCommon/WeakPtrs/WeakPtrs.h"
#include "Math/Vector2/Point2D.h"
#include "Math/Vector3/Vector3.h"
#include "Syncronization/Mutex.h"
#include "Syncronization/ScopeLock.h"

#include "vector"
#include <math.h>
#include <string>

struct Driveable {
	
	Driveable() {};
	
	WeakPtrs<GameObject> obj;
	float mass;
	Engine::Math::Vector3 force;
	float dragCoefficient;

	//standard constructor
	Driveable(WeakPtrs<GameObject> i_game_object, const Engine::Math::Vector3 & i_force, float i_mass, float i_drag_coefficient) :
	obj(i_game_object), mass(i_mass), force(i_force), dragCoefficient(i_drag_coefficient)
	{
		
	}
	
	//copy constructor
	Driveable(const Driveable & i_Other) :
	obj(i_Other.obj), mass(i_Other.mass), force(i_Other.force), dragCoefficient(i_Other.dragCoefficient)
	{
		
	}
};

class Physics {
public:
	inline static void Initialize() { /*Physics::p_Instance->objsToDrive = std::vector<Driveable>();*/ };

	static bool AddGameObjectToPhysics(SmartPtrs<GameObject> i_game_object, const Engine::Math::Vector3 & i_force, float i_mass, float i_drag_coefficient);

	static void MoveObjects();

	//may require delta time param
	static void Run(float i_dt);

	static void Update(WeakPtrs<GameObject> io_Object, const Engine::Math::Vector3 & i_Forces, float i_dt, Driveable & driveable);

	static Engine::Math::Vector3 CalculateForce(Driveable & driveable);

	static void UpdateForce(Engine::Math::Vector3 i_force, std::string i_name);
	
	inline static void Shutdown() { delete Physics::p_Instance; };


private:
	static Physics	        *p_Instance;
	Physics() {};
	std::vector<Driveable>	objsToDrive;
	std::vector<Driveable>	newObjsToDrive;
};
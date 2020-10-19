#include "Physics.h"

Engine::Mutex mutexPhysicsLock;

Physics * Physics::p_Instance = new Physics();

bool Physics::AddGameObjectToPhysics(SmartPtrs<GameObject> i_game_object, const Engine::Math::Vector3 & i_force, float i_mass, float i_drag_coefficient) {

	Engine::ScopeLock scopeLock(mutexPhysicsLock);

	i_game_object->SetMass(i_mass);
	
	Physics::p_Instance->newObjsToDrive.emplace_back(WeakPtrs<GameObject>(i_game_object) , i_force, i_mass, i_drag_coefficient);

	return true;
}

void Physics::MoveObjects()
{
	Engine::ScopeLock scopeLock(mutexPhysicsLock);
	
	for (int i = 0; i < p_Instance->newObjsToDrive.size(); ++i)
	{
		p_Instance->objsToDrive.push_back(p_Instance->newObjsToDrive[i]);
	}

	p_Instance->newObjsToDrive.clear();
}


void Physics::UpdateForce(Engine::Math::Vector3 i_force, std::string i_name)
{
	
	for (int i = 0; i < p_Instance->objsToDrive.size(); i++)
	{
		if (!i_name.compare(p_Instance->objsToDrive[i].obj.Acquire()->GetName()))
		{
			p_Instance->objsToDrive[i].force = i_force;
		}
	}
	
}


void Physics::Run(float i_dt) {

	MoveObjects();
	
	for (int i=0; i < p_Instance->objsToDrive.size(); ++i)
	{
		Update(p_Instance->objsToDrive[i].obj, CalculateForce(p_Instance->objsToDrive[i]), i_dt, p_Instance->objsToDrive[i]);
	}
}

Engine::Math::Vector3 Physics::CalculateForce(Driveable & driveable) {

	float dragVal = driveable.dragCoefficient;

	if (driveable.obj.Acquire()->GetVelocity().x() == 0.0f)
	{
		if (driveable.obj.Acquire()->GetVelocity().y() > 0.0f)
		{
			dragVal = -dragVal;
		}
	}
	else if (driveable.obj.Acquire()->GetVelocity().x() > 0.0f)
	{
		dragVal = -dragVal;
	}
	
	return driveable.force + ((dragVal) * (driveable.obj.Acquire()->GetVelocity() *
		driveable.obj.Acquire()->GetVelocity()));

}


void Physics::Update(WeakPtrs<GameObject> io_Object, const Engine::Math::Vector3& i_Force, float i_dt, Driveable & driveable) {

	Engine::Math::Vector3 const Acceleration = i_Force / driveable.mass;

	Engine::Math::Vector3 const prevVelocity = io_Object.Acquire()->GetVelocity();
	Engine::Math::Vector3 const currVelocity = prevVelocity + Acceleration * i_dt;

	io_Object.Acquire()->SetVelocity(currVelocity);

	Engine::Math::Vector3 const prevPosition = io_Object.Acquire()->GetPosition();
	Engine::Math::Vector3 const currPosition = prevPosition + ((prevVelocity + currVelocity) / 2) * i_dt;

	/*const size_t	lenBuffer = 65;
	char			Buffer[lenBuffer];

	sprintf_s(Buffer, lenBuffer, "current vel %f - %f\n", i_Force.x(),i_Force.y());
	OutputDebugStringA(Buffer);*/

	io_Object.Acquire()->SetPosition(currPosition);
}

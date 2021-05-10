#include "Collision.h"

Engine::Mutex mutexCollisionLock;	// using a mutex to deal with multiple threads running at the same time

Collision * Collision::c_Instance = new Collision();

/**
* Stores new gameobjects that are affected by collision to a vector
*
* @param i_game_object smart pointer referencing to a game object in the world
*/
void Collision::AddGameObjectToCollision(SmartPtrs<GameObject> i_game_object) {

	Engine::ScopeLock scopeLock(mutexCollisionLock);        // locking the mutex using a scoped lock so it can automatically be unlocked when tasks on this thread have completed

	if (i_game_object)
	{
        // Converts the smart pointer pointing to game object to a weak pointer pointing to that game object
        // and adds it to the collision affected game objects vector 
		Collision::c_Instance->newObjsToCollide.emplace_back(WeakPtrs<GameObject>(i_game_object));
	}
}

/**
* Copies over any newly added game objects that are affected by collision to a vector storing 
* all game objects affected by collision
*/
void Collision::MoveObjects()
{
	Engine::ScopeLock scopeLock(mutexCollisionLock);
	
	for (int i = 0; i < c_Instance->newObjsToCollide.size(); ++i)
	{
		c_Instance->objsToCollide.push_back(c_Instance->newObjsToCollide[i]);
	}

	c_Instance->newObjsToCollide.clear();
}

/**
* Runs the game loop every frame checking for and handling collisions between objects
* @param i_dt Delta Time
*/
void Collision::Run(float i_dt) {

	MoveObjects();

	bool breakOut = false;

	CollisionPair EarliestCollision;

	EarliestCollision.collisionTime = i_dt;
	
	for (int i = 0; i < c_Instance->objsToCollide.size(); ++i)
	{
		if (!breakOut)
		{
			for (int j = i + 1; j < c_Instance->objsToCollide.size(); ++j)
			{
				if (!breakOut)
				{
					Engine::Math::Vector4 mCollisionNormal = Engine::Math::Vector4::Zero;
					float mCollisionTime = 0.0;
					
					//check for collision of every collidable obj with every other collidable obj (without swapped repeats)
					bool result = CheckForCollision(c_Instance->objsToCollide[i], c_Instance->objsToCollide[j], i_dt, mCollisionNormal, mCollisionTime);
                    // collision has been found between two collidables
					if (result)
					{
						std::string obj_A = c_Instance->objsToCollide[i].obj.Acquire()->GetName();
						std::string obj_B = c_Instance->objsToCollide[j].obj.Acquire()->GetName();

						// If the player collides with any of the monsters end the game
						if ((obj_A == "Player" || obj_A == "Monster 2") && (obj_B == "Player" || obj_B == "Monster 2"))
						{
							c_Instance->exitOnCollide = true;
							breakOut = true;
							break;
						}
                        // If the player collides with any of the monsters end the game
						if ((obj_A == "Player" || obj_A == "Monster 1") && (obj_B == "Player" || obj_B == "Monster 1"))
						{
							c_Instance->exitOnCollide = true;
							breakOut = true;
							break;
						}

                        // Store all the collision information from this collision
						if (mCollisionTime < EarliestCollision.collisionTime)
						{
							EarliestCollision.collisionTime = mCollisionTime;
							EarliestCollision.collisionNormal = mCollisionNormal;
							EarliestCollision.objA = c_Instance->objsToCollide[i];
							EarliestCollision.objB = c_Instance->objsToCollide[j];
						}
						// deals with the collision response
						CollisionResponse(EarliestCollision.collisionNormal, EarliestCollision.objA, EarliestCollision.objB, EarliestCollision.collisionTime);

                        // Debugging info to print collision check results
						const size_t	lenBuffer = 65;
						char			Buffer[lenBuffer];

						sprintf_s(Buffer, lenBuffer, "\ncollision: %d\n", result);
						OutputDebugStringA(Buffer);
					}
				}
			}
		}
		else
		{
			break;
		}
	}
}

/**
* Checks if two collidable objects have collided or not
* @param i_A Reference to first collidable obj
* @param i_B Reference to second collidable obj
* @param i_dt Delta Time
* @param i_collisionNormal Reference to the collision normal vector between the two collidables
* @param i_collisionTime Reference to the time when collision occured between the two collidables
* @return TRUE - Collision has occured.
*         FALSE - No collision has occured.
*/
bool Collision::CheckForCollision(Collidable i_A, Collidable& i_B, float i_dt, Engine::Math::Vector4 & i_collisionNormal, float & i_collisionTime)
{
	using namespace Engine::Math;

	Vector4 mCollisionNormalLocal = Vector4::Zero;

	std::vector<float> t_Open;
	std::vector<float> t_Close;
	
	////// CALCULATIONS FOR OBJECT A //////

	// A's Velocity in A's local coordinate system
	Vector3 mVelocity_A = i_A.obj.Acquire()->GetVelocity();
	// A's BB Center in A's local coordinate system
	Vector4 BBCenter_A = Vector4(i_A.obj.Acquire()->GetBBCenter().x(), i_A.obj.Acquire()->GetBBCenter().y(), .0f, 1.0f);
	// A's X and Y Extents in local coordinate system
	Point2D Extents_A = i_A.obj.Acquire()->GetExtents();

	if (Helpers::IsNaN(mVelocity_A.x()))
	{
		bool trouble = true;
	}
	
	Matrix4 mRot_A = Matrix4::CreateZRotation(i_A.obj.Acquire()->GetRotation());
	Matrix4 mTrans_A = Matrix4::CreateTranslation(i_A.obj.Acquire()->GetPosition());

	// A's conversion matrix from A's local to world coordinate system
	Matrix4 mAToWorld = mTrans_A * mRot_A;
	// A's conversion matrix from world to A's local coordinate system
	Matrix4 mWorldToA = mAToWorld.GetInverse();
	
	// A's BB Center in world coordinate system
	Vector4 BBCenterInWorld_A = mAToWorld.MultiplyRight(BBCenter_A);


	////// CALCULATIONS FOR OBJECT B //////

	// B's Velocity in B's local coordinate system
	Vector3 mVelocity_B = i_B.obj.Acquire()->GetVelocity();
	// B's BB Center in B's local coordinate system
	Vector4 BBCenter_B = Vector4(i_B.obj.Acquire()->GetBBCenter().x(), i_B.obj.Acquire()->GetBBCenter().y(), .0f, 1.0f);
	// B's X and Y Extents in local coordinate system
	Point2D Extents_B = i_B.obj.Acquire()->GetExtents();

	Matrix4 mRot_B = Matrix4::CreateZRotation(i_B.obj.Acquire()->GetRotation());
	Matrix4 mTrans_B = Matrix4::CreateTranslation(i_B.obj.Acquire()->GetPosition());

	// B's conversion matrix from B's local to world coordinate system
	Matrix4 mBToWorld = mTrans_B * mRot_B;
	// B's conversion matrix from world to B's local coordinate system
	Matrix4 mWorldToB = mBToWorld.GetInverse();
	
	// B's BB Center in world coordinate system
	Vector4 BBCenterInWorld_B = mBToWorld.MultiplyRight(BBCenter_B);


	////// MOVING BETWEEN A's & B's COORDINATE SYSTEM //////

	// A's conversion matrix from A's local coordinate system to B's local coordinate system
	Matrix4 mAToB = mWorldToB * mAToWorld;
	// A's BB Center in B's local coordinate system
	Vector4 BBCenterInB_A = mAToB.MultiplyRight(BBCenter_A);

	// B's conversion matrix from B's local coordinate system to A's local coordinate system
	Matrix4 mBToA = mWorldToA * mBToWorld;
	// B's BB Center in A's local coordinate system
	Vector4 BBCenterInA_B = mBToA.MultiplyRight(BBCenter_B);

	// A's X & Y extents in B's local coordinate system
	Vector4 ExtentsXInB_A = mAToB.MultiplyRight(Vector4(Extents_A.x(), .0f, .0f, .0f));
	Vector4 ExtentsYInB_A = mAToB.MultiplyRight(Vector4(.0f, Extents_A.y(), .0f, .0f));

	float AProjectionOntoB_X = fabs(ExtentsXInB_A.x()) + fabs(ExtentsYInB_A.x());
	float AProjectionOntoB_Y = fabs(ExtentsXInB_A.y()) + fabs(ExtentsYInB_A.y());

	// B's X & Y extents in A's local coordinate system
	Vector4 ExtentsXInA_B = mBToA.MultiplyRight(Vector4(Extents_B.x(), .0f, .0f, .0f));
	Vector4 ExtentsYInA_B = mBToA.MultiplyRight(Vector4(.0f, Extents_B.y(), .0f, .0f));

	float BProjectionOntoA_X = fabs(ExtentsXInA_B.x()) + fabs(ExtentsYInA_B.x());
	float BProjectionOntoA_Y = fabs(ExtentsXInA_B.y()) + fabs(ExtentsYInA_B.y());


	////// CHECK A AGAINST B IN B's COORDINATE SYSTEM //////

	// Check for separation along B's x axis
	bool isSeperated_Bx = fabs(BBCenterInB_A.x() - BBCenter_B.x()) > Extents_B.x() + AProjectionOntoB_X;
	// Check for separation along B's y axis
	bool isSeperated_By = fabs(BBCenterInB_A.y() - BBCenter_B.y()) > Extents_B.y() + AProjectionOntoB_Y;

	////// CHECK B AGAINST A IN A's COORDINATE SYSTEM //////

	// Check for separation along B's x axis
	bool isSeperated_Ax = fabs(BBCenterInA_B.x() - BBCenter_A.x()) > Extents_A.x() + BProjectionOntoA_X;
	// Check for separation along B's y axis
	bool isSeperated_Ay = fabs(BBCenterInA_B.y() - BBCenter_A.y()) > Extents_A.y() + BProjectionOntoA_Y;


	////// WORKING WITH VELOCITIES DURING CALCULATION -- MOVING OBJECTS //////

	Vector3 VelARelToB = mVelocity_A - mVelocity_B;
	// A's Velocity in B's local coordinate system
	Vector4 VelAInB = mWorldToB.MultiplyRight(Vector4(VelARelToB.x(), VelARelToB.y(), VelARelToB.z(), .0f));

	Vector3 VelBRelToA = mVelocity_B - mVelocity_A;
	// B's Velocity in A's local coordinate system
	Vector4 VelBInA = mWorldToA.MultiplyRight(Vector4(VelBRelToA.x(), VelBRelToA.y(), VelBRelToA.z(), .0f));

	

	// Check A against B in B's x axis -- 1st check
	float FinalExtentX_B = Extents_B.x() + AProjectionOntoB_X;
	float BLeft_X = BBCenter_B.x() - FinalExtentX_B;
	float BRight_X = BBCenter_B.x() + FinalExtentX_B;

	float D_Close_Bx = BLeft_X - BBCenterInB_A.x();
	float D_Open_Bx = BRight_X - BBCenterInB_A.x();

	float t_Close_Bx = 0;
	float t_Open_Bx = 0;

	// Edge case: Velocity = 0
	if (VelAInB.x() == 0)
	{
		if (!(BBCenterInB_A.x() > BLeft_X && BBCenterInB_A.x() < BRight_X))
		{
			return false;
		}
	}
	else
	{
		t_Close_Bx = D_Close_Bx / VelAInB.x();
		t_Open_Bx = D_Open_Bx / VelAInB.x();

		// Edge case: Swap t_open and t_close if t_open is less than t_close
		if (t_Open_Bx < t_Close_Bx)
		{
			float temp = t_Open_Bx;
			t_Open_Bx = t_Close_Bx;
			t_Close_Bx = temp;
		}

		t_Open.push_back(t_Open_Bx);
		t_Close.push_back(t_Close_Bx);
	}
	
	// Check A against B in B's y axis -- 2nd check
	float FinalExtentY_B = Extents_B.y() + AProjectionOntoB_Y;
	float BLeft_Y = BBCenter_B.y() - FinalExtentY_B;
	float BRight_Y = BBCenter_B.y() + FinalExtentY_B;

	float D_Close_By = BLeft_Y - BBCenterInB_A.y();
	float D_Open_By = BRight_Y - BBCenterInB_A.y();

	float t_Close_By = 0;
	float t_Open_By = 0;

	// Edge case: Velocity = 0
	if (VelAInB.y() == 0)
	{
		if (!(BBCenterInB_A.y() > BLeft_Y && BBCenterInB_A.y() < BRight_Y))
		{
			return false;
		}
	}
	else
	{
		t_Close_By = D_Close_By / VelAInB.y();
		t_Open_By = D_Open_By / VelAInB.y();

		// Edge case: Swap t_open and t_close if t_open is less than t_close
		if (t_Open_By < t_Close_By)
		{
			float temp = t_Open_By;
			t_Open_By = t_Close_By;
			t_Close_By = temp;
		}

		t_Open.push_back(t_Open_By);
		t_Close.push_back(t_Close_By);
	}

	// Check B against A in A's x axis -- 3rd check
	float FinalExtentX_A = Extents_A.x() + BProjectionOntoA_X;
	float ALeft_X = BBCenter_A.x() - FinalExtentX_A;
	float ARight_X = BBCenter_A.x() + FinalExtentX_A;

	float D_Close_Ax = ALeft_X - BBCenterInA_B.x();
	float D_Open_Ax = ARight_X - BBCenterInA_B.x();

	float t_Close_Ax = 0;
	float t_Open_Ax = 0;

	// Edge case: Velocity = 0
	if (VelBInA.x() == 0)
	{
		if (!(BBCenterInA_B.x() > ALeft_X && BBCenterInA_B.x() < ARight_X))
		{
			return false;
		}
	}
	else
	{
		t_Close_Ax = D_Close_Ax / VelBInA.x();
		t_Open_Ax = D_Open_Ax / VelBInA.x();

		// Edge case: Swap t_open and t_close if t_open is less than t_close
		if (t_Open_Ax < t_Close_Ax)
		{
			float temp = t_Open_Ax;
			t_Open_Ax = t_Close_Ax;
			t_Close_Ax = temp;
		}

		t_Open.push_back(t_Open_Ax);
		t_Close.push_back(t_Close_Ax);
	}

	// Check B against A in A's y axis -- 4th check
	float FinalExtentY_A = Extents_A.y() + BProjectionOntoA_Y;
	float ALeft_Y = BBCenter_A.y() - FinalExtentY_A;
	float ARight_Y = BBCenter_A.y() + FinalExtentY_A;

	float D_Close_Ay = ALeft_Y - BBCenterInA_B.y();
	float D_Open_Ay = ARight_Y - BBCenterInA_B.y();

	float t_Close_Ay = 0;
	float t_Open_Ay = 0;

	// Edge case: Velocity = 0
	if (VelBInA.y() == 0)
	{
		if (!(BBCenterInA_B.y() > ALeft_Y && BBCenterInA_B.y() < ARight_Y))
		{
			return false;
		}
	}
	else
	{
		t_Close_Ay = D_Close_Ay / VelBInA.y();
		t_Open_Ay = D_Open_Ay / VelBInA.y();

		// Edge case: Swap t_open and t_close if t_open is less than t_close
		if (t_Open_Ay < t_Close_Ay)
		{
			float temp = t_Open_Ay;
			t_Open_Ay = t_Close_Ay;
			t_Close_Ay = temp;
		}

		t_Open.push_back(t_Open_Ay);
		t_Close.push_back(t_Close_Ay);
	}

	
	//// DETERMINE IF THERE WAS ANY COLLISION BETWEEN A AND B BASED ON VELOCITY //////

	//Edge case: t-open < 0
	for (int i = 0; i < t_Open.size(); i++)
	{
		// A collision happened in the past
		if (t_Open[i] < 0)
		{
			return false;
		}
	}

	//Edge case: t-close > frame-time
	for (int i = 0; i < t_Close.size(); i++)
	{
		// A collision may happen in a future frame
		if (t_Close[i] > i_dt)
		{
			return false;
		}
	}

	float t_Open_E = t_Open[0];
	float t_Close_L = t_Close[0];
	

	// track the earliest t open
	for (int i = 0; i < t_Open.size(); i++)
	{

		if (t_Open[i] < t_Open_E)
		{
			t_Open_E = t_Open[i];
		}


	}

	// track the latest t close
	for (int i = 0; i < t_Close.size(); i++)
	{

		if (t_Close[i] > t_Close_L)
		{
			t_Close_L = t_Close[i];
		}


	}
	
	// If latest t close was after earliest t open then a new separation opened before the last separation closed -- NO COLLISION
	if (t_Close_L > t_Open_E)
	{
		return false;
	}

	//// DETERMINE WHICH AXIS COLLISION OCCURED ON, SET COLLISION NORMAL VECTOR, AND SET COLLISION TIME //////
	
	// Obj A colliding with obj B on B's x axis
	if (t_Close_L == t_Close_Bx)
	{
		mCollisionNormalLocal = Vector4(1.0f, 0.0f, 0.0f, 0.0f);

		i_collisionNormal = mBToWorld.MultiplyRight(mCollisionNormalLocal);

		i_collisionNormal = Normalize(i_collisionNormal);
	}
	// Obj A colliding with obj B on B's y axis
	else if (t_Close_L == t_Close_By)
	{
		mCollisionNormalLocal = Vector4(0.0f, 1.0f, 0.0f, 0.0f);

		i_collisionNormal = mBToWorld.MultiplyRight(mCollisionNormalLocal);

		i_collisionNormal = Normalize(i_collisionNormal);
	}
	// obj B colliding with obj A on A's x axis
	else if (t_Close_L == t_Close_Ax)
	{
		mCollisionNormalLocal = Vector4(1.0f, 0.0f, 0.0f, 0.0f);

		i_collisionNormal = mAToWorld.MultiplyRight(mCollisionNormalLocal);

		i_collisionNormal = Normalize(i_collisionNormal);
	}
	// obj B colliding with obj A on A's y axis
	else if (t_Close_L == t_Close_Ay)
	{
		mCollisionNormalLocal = Vector4(0.0f, 1.0f, 0.0f, 0.0f);

		i_collisionNormal = mAToWorld.MultiplyRight(mCollisionNormalLocal);

		i_collisionNormal = Normalize(i_collisionNormal);
	}
	
	// If latest t close was before earliest t open then all separation closed before a new separation opened -- COLLISION
	i_collisionTime = t_Close_L;
	return true;
}

/**
* Handles what happens when the two objects collide
* @param i_CollisionNormal The collision normal vector between the two collidables
* @param i_A Reference to first collidable obj
* @param i_B Reference to second collidable obj
* @param i_CollisionTime Reference to the time when collision occured between the two collidables
*/
void Collision::CollisionResponse(Engine::Math::Vector4 i_CollisionNormal, Collidable & i_A, Collidable & i_B, float i_CollisionTime)
{
	using namespace Engine::Math;

    // velocities of the two collidables before collision response
	Vector3 oldVelocity_A = i_A.obj.Acquire()->GetVelocity();
	Vector3 oldVelocity_B = i_B.obj.Acquire()->GetVelocity();

	// ARE THE OBJECTS MOVING TOWARDS COLLISION NORMAL? IF YES, REFLECT THEM. OTHERWISE, DO NOTHING

	// After collision velocity for object A
	float left_side = (i_A.obj.Acquire()->GetMass() - i_B.obj.Acquire()->GetMass()) / (i_A.obj.Acquire()->GetMass() + i_B.obj.Acquire()->GetMass());
	float right_side = (2.0f * i_B.obj.Acquire()->GetMass()) / (i_A.obj.Acquire()->GetMass() + i_B.obj.Acquire()->GetMass());

	Vector3 newVelocity_A = (left_side * oldVelocity_A) + (right_side * oldVelocity_B);

	i_A.obj.Acquire()->SetVelocity(newVelocity_A);

	// After collision velocity for object B
	float left_side_1 = (i_B.obj.Acquire()->GetMass() - i_A.obj.Acquire()->GetMass()) / (i_A.obj.Acquire()->GetMass() + i_B.obj.Acquire()->GetMass());
	float right_side_1 = (2.0f * i_A.obj.Acquire()->GetMass()) / (i_A.obj.Acquire()->GetMass() + i_B.obj.Acquire()->GetMass());

	Vector3 newVelocity_B = (left_side_1 * oldVelocity_B) + (right_side_1 * oldVelocity_A);

	i_B.obj.Acquire()->SetVelocity(newVelocity_B);

	//Simple reflection for object A
	if( dot(Vector4(i_A.obj.Acquire()->GetVelocity(), 0.0f), i_CollisionNormal) < 0.0f )
	{
		Vector4 left_side_eq = -1.0f * Vector4(i_A.obj.Acquire()->GetVelocity(), 0.0f);
		Vector4 right_side_eq = 2.0f * (i_CollisionNormal * (dot(i_CollisionNormal, (-1.0f * Vector4(i_A.obj.Acquire()->GetVelocity(), 0.0f)))) 
								- Vector4(i_A.obj.Acquire()->GetVelocity(), 0.0f));
		Vector4 relfectVelocity_A_v4 = left_side_eq - right_side_eq;
		Vector3 finalReflectVelocity_A = Vector3(relfectVelocity_A_v4.x(), relfectVelocity_A_v4.y(), relfectVelocity_A_v4.z());

        // new velocity for object A after collision response
		i_A.obj.Acquire()->SetVelocity(finalReflectVelocity_A);
	}

	//Simple reflection for object B
	if (dot(Vector4(i_B.obj.Acquire()->GetVelocity(), 0.0f), i_CollisionNormal) < 0.0f)
	{
		Vector4 left_side_eq = -1.0f * Vector4(i_B.obj.Acquire()->GetVelocity(), 0.0f);
		Vector4 right_side_eq = 2.0f * (i_CollisionNormal * (dot(i_CollisionNormal, (-1.0f * Vector4(i_B.obj.Acquire()->GetVelocity(), 0.0f))))
			- Vector4(i_B.obj.Acquire()->GetVelocity(), 0.0f));
		Vector4 relfectVelocity_B_v4 = left_side_eq - right_side_eq;
		Vector3 finalReflectVelocity_B = Vector3(relfectVelocity_B_v4.x(), relfectVelocity_B_v4.y(), relfectVelocity_B_v4.z());

        // new velocity for object B after collision response
		i_B.obj.Acquire()->SetVelocity(finalReflectVelocity_B);
	}
}

#pragma once
#include "Renderer/Renderer.h"
#include "Physics/Physics.h"
#include "Collision/Collision.h"
#include "Input/Input.h"
#include "Timing/Timing.h"
#include "GameCommon/GameObject/GameObject.h"
#include "GLib.h"
#include "Helpers/Helpers.h"
#include "JobSystem/JobSystem.h"
#include "JobSystem/IJob.h"
#include "Syncronization/Events.h"
#include "Math/Matrix4/Matrix4.h"
#include "GameCommon/GameObject/Manager.h"

namespace Engine
{
	void Init();
	void Run();
	void Shutdown();
};



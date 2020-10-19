#pragma once
#include "GLib.h"
#include "Physics/Physics.h"
//KeyCodes
#define KC_D 0x44
#define KC_A 0x41
#define KC_W 0x57
#define KC_S 0x53
#define KC_Q 0x51

class Input
{
public:
	static void TestKeyCallback(unsigned int i_VKeyID, bool bWentDown);
	
	inline static void CheckForInput() { GLib::SetKeyStateChangeCallback(TestKeyCallback); };
	
	inline static bool QuitRequested() { return i_Instance->QuitGame; };
	
	static unsigned int key()
	{
		return i_Instance->m_keyPressed;
	}
	
	static inline bool& GetIsKeyDown()
	{
		return i_Instance->IsKeyDown;
	}
	
	static inline unsigned int& GetKeyPressed()
	{
		return i_Instance->m_keyPressed;
	}
	
	static inline bool GetKeyReleased()
	{
		return (!i_Instance->IsKeyDown);
	}
	
	
private:
	unsigned int m_keyPressed;
	bool IsKeyDown;
	bool QuitGame = false;
	Input() {};
	static Input * i_Instance;
};

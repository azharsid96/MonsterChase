#include "Input.h"

#include <stdio.h>
#include <Windows.h>

Input * Input::i_Instance = new Input();

void Input::TestKeyCallback(unsigned int i_VKeyID, bool bWentDown)
{
if(bWentDown==false)
{
	printf("scd") ;
}
	i_Instance->GetKeyPressed() = i_VKeyID;
	i_Instance->GetIsKeyDown() = bWentDown;

	const size_t	lenBuffer = 65;
	char			Buffer[lenBuffer];
	//Press 'Q' key
	if (i_VKeyID == 81)
		i_Instance->QuitGame = true;
	sprintf_s(Buffer, lenBuffer, "VKey 0x%04x went %s\n", i_VKeyID, bWentDown ? "down" : "up");
	OutputDebugStringA(Buffer);

}





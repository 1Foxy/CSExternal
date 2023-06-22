#pragma once

#include <Windows.h>

class Aimbot
{
public:
	
	int selectedTargetBoneIndex = 8;
	// head
	void __fastcall aimbot();
private:
	bool __fastcall MaxLockDistance(DWORD localPlayer, DWORD entity);
	bool __fastcall IsDead(DWORD entity);
	bool __fastcall IsCrouchingOrInAir(DWORD entity);
	DWORD localPlayer = 0;
}; extern Aimbot oAimbot;

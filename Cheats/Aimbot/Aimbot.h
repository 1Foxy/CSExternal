#pragma once

#include <Windows.h>

class Aimbot
{
public:
	
	int selectedTargetBoneIndex = 8; // head
	void aimbot();
private:
	bool __fastcall MaxLockDistance(DWORD localPlayer, DWORD entity);
	bool __fastcall IsDead(DWORD entity);
	bool __fastcall IsCrouchingOrInAir(DWORD entity);
}; extern Aimbot oAimbot;

#pragma once

#include <Windows.h>

class TriggerBot
{
public:
	void __fastcall AutoShootOnEnemy();
private:
		DWORD LocalPlayer = 0;
};extern TriggerBot oTriggerBot;


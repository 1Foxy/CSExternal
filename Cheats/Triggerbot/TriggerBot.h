#pragma once

#include <Windows.h>

class TriggerBot
{
public:
	void __fastcall AutoShootOnEnemy();
	void __fastcall init();
private:
		DWORD LocalPlayer = 0;
};extern TriggerBot oTriggerBot;


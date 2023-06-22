#pragma once
#include "../Cheats/RCS/RecoilControlSystem.h"
#include "../Cheats/Aimbot/Aimbot.h"
#include "../Cheats/Triggerbot/TriggerBot.h"

namespace INIT
{
	void Initialize()
	{
	  oRecoilControlSystem.InitRCS();
	  oAimbot.init();
	  oTriggerBot.init();
	}
}
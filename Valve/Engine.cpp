#include "Engine.h"
#include <stdio.h>
#include "../Header Files/memory.hpp"

namespace Engine {
	bool Initialize() {
		g_game_base = reinterpret_cast<std::uintptr_t>(mem.get_module("csgo.exe").modBaseAddr);
		if (!g_game_base)
			return false;
		else
			printf("csgo.exe -> 0x%x\n", g_game_base);

		g_engine = reinterpret_cast<std::uintptr_t>(mem.get_module("engine.dll").modBaseAddr);
		if(!g_engine)
			return false;
		else
			printf("engine.dll -> 0x%x\n", g_engine);

		g_client_base = reinterpret_cast<std::uintptr_t>(mem.get_module("client.dll").modBaseAddr);
		if (!g_client_base)
			return false;
		else
			printf("client.dll -> 0x%x\n", g_client_base);

		g_vstlib = reinterpret_cast<std::uintptr_t>(mem.get_module("vstdlib.dll").modBaseAddr);
		if (!g_vstlib)
			return false;
		else
			printf("vstdlib.dll -> 0x%x\n", g_vstlib);


		printf("Engine Init!\n\n");
		return true;
	}
}

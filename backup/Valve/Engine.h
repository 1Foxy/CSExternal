#pragma once
#include <cstdint>

inline std::uintptr_t g_engine;
inline std::uintptr_t g_client_base;
inline std::uintptr_t g_game_base;

namespace Engine
{
	enum WEAPON_TYPE;

	bool		Initialize();

}

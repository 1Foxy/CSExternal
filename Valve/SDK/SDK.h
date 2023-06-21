#pragma once
#include "../../Header Files/common.h"
#include "../../Header Files/Hazedumper.h"
#include "../Engine.h"
#include "Vector3.h"


namespace SDK
{
	class c_class
	{
	public:

		std::uintptr_t get_address()
		{
			return reinterpret_cast<std::uintptr_t>(this);
		}
		bool is_valid()
		{
			return mem.is_valid(get_address());
		}

	};

	class c_player : public c_class
	{
	public:

		int team()
		{
			return mem.read<int>(get_address() + hazedumper::netvars::m_iTeamNum);
		}

		int health()
		{
			return mem.read<int>(get_address() + hazedumper::netvars::m_iHealth);
		}

		Vector3 origin()
		{
			return mem.read<Vector3>(get_address() + hazedumper::netvars::m_vecOrigin);
		}

		int life_state()
		{
			return mem.read<int>(get_address() + hazedumper::netvars::m_lifeState);
		}

		int glow_index()
		{
			return mem.read<int>(get_address() + hazedumper::netvars::m_iGlowIndex);
		}

		Vector3 velocity()
		{
			return mem.read<Vector3>(get_address() + hazedumper::netvars::m_vecVelocity);
		}

		static c_player* find(int idx)
		{
			return mem.read<c_player*>(g_client_base + hazedumper::signatures::dwEntityList + (idx * 0x10));
		}

		int GetFlags()
		{
			return mem.read<int>(get_address() + hazedumper::netvars::m_fFlags);
		}

		//int getLocalCrosshairID()
		//{
		//	return mem.read<int>(GetLocalPlayer() + hazedumper::netvars::m_iCrosshairId);
		//}

		float GetFlashTime() {
			return mem.read<float>(get_address() + hazedumper::netvars::m_flFlashDuration);
		}

		void SetFlashTime(float duration) {
			uintptr_t flashDurationAddress = get_address() + hazedumper::netvars::m_flFlashDuration;
			mem.write<float>(flashDurationAddress, duration);
		}

		D3DXVECTOR3 getLocalViewAngles()
		{
			return mem.read<D3DXVECTOR3>(g_engine + hazedumper::signatures::dwClientState_ViewAngles);
		}

		void setLocalViewAngles(D3DXVECTOR3 angles)
		{
			mem.write<D3DXVECTOR3>(g_engine + hazedumper::signatures::dwClientState_ViewAngles, angles);
		}

		bool isScoped()
		{
			return mem.read<bool>(get_address() + hazedumper::netvars::m_bIsScoped);
		}

		bool IsEntityUnderCrosshair()
		{
			// Get the local player entity from hazedumper
			uintptr_t localPlayer = *(uintptr_t*)(get_address() + hazedumper::signatures::dwLocalPlayer);

			// Get the crosshair ID from the local player
			int crosshairID = *(int*)(localPlayer + hazedumper::netvars::m_iCrosshairId);

			if (crosshairID != 0 && crosshairID < 64)
			{
				// Get the entity from the entity list based on crosshair ID
				uintptr_t entity = *(uintptr_t*)(get_address() + hazedumper::signatures::dwEntityList + (crosshairID - 1) * 0x10);

				// Check if the entity is valid and alive
				if (entity != 0 && *(int*)(entity + hazedumper::netvars::m_iHealth) > 0)
				{
					return true; // Entity is under crosshair and meets criteria
				}
			}

			return false; // No entity found under crosshair
		}



	};
	extern c_player cplayer;

	inline c_player* get_local_player()
	{
		return mem.read<c_player*>(g_client_base + hazedumper::signatures::dwLocalPlayer);
	}


}
#include "TriggerBot.h"
#include "../../Valve/SDK/SDK.h"


void TriggerBot::AutoShootOnEnemy()
{
    if (Globals::Triggerbot) {
        // Get the local player's crosshair ID
        int crosshairId = mem.read<int>(LocalPlayer + hazedumper::netvars::m_iCrosshairId);

        // Check if the crosshair ID is valid and nonzero
        if (crosshairId > 0 && crosshairId <= 64) {
            // Get the entity index of the player at the crosshair
            int entityIndex = mem.read<int>(g_client_base + hazedumper::signatures::dwEntityList + (crosshairId - 1) * 0x10);

            // Check if the entity index is valid
            if (entityIndex > 0) {
                // Get the team number of the local player
                int localPlayerTeam = mem.read<int>(LocalPlayer + hazedumper::netvars::m_iTeamNum);

                // Get the team number of the player at the crosshair
                int enemyTeam = mem.read<int>(entityIndex + hazedumper::netvars::m_iTeamNum);

                // Check if the player at the crosshair is an enemy
                if (localPlayerTeam != enemyTeam) {
                    Sleep(Globals::autoShootDelay);
                    mem.write<int>(g_client_base + hazedumper::signatures::dwForceAttack, 6);
                }
            }
        }
    }
}

void __fastcall TriggerBot::init()
{
    LocalPlayer = mem.read<DWORD>(g_client_base + hazedumper::signatures::dwLocalPlayer);
}

TriggerBot oTriggerBot;
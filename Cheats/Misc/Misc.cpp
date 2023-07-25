#include "Misc.h"
#include "../../Valve/SDK/SDK.h"
#include "../../imgui/imgui.h"

void Misc::Bunnyhop()
{
    auto local_player = SDK::get_local_player();
    if (!local_player)
        return;
    if (Globals::bhop)
    {
        if (GetAsyncKeyState(VK_SPACE) & 0x8000 && (local_player->GetFlags() & 1 << 0)) //(1<<0) this means FL_ONGROUND
        {
            mem.write<int>(g_client_base + hazedumper::signatures::dwForceJump, 5);
        }
        else if (GetAsyncKeyState(VK_SPACE) & 0x8000 && !(local_player->GetFlags() & 1 << 0))
        {
            mem.write<int>(g_client_base + hazedumper::signatures::dwForceJump, 4);
            mem.write<int>(g_client_base + hazedumper::signatures::dwForceJump, 5);
            mem.write<int>(g_client_base + hazedumper::signatures::dwForceJump, 4);
        }
        else
        {
            mem.write<int>(g_client_base + hazedumper::signatures::dwForceJump, 4);
        }
    }
}


Misc oMisc;



#include "Misc.h"
#include "../../Valve/SDK/Const.h"
#include "../../Valve/SDK/SDK.h"

void Misc::Bunnyhop()
{
    auto local_player = SDK::get_local_player();
    if (!local_player)
        return;
    if (Globals::bhop)
    {
        if (GetAsyncKeyState(VK_SPACE) & 0x8000 && (local_player->GetFlags() & FL_ONGROUND))
        {
            mem.write<int>(g_client_base + hazedumper::signatures::dwForceJump, 5);
        }
        else if (GetAsyncKeyState(VK_SPACE) & 0x8000 && !(local_player->GetFlags() & FL_ONGROUND))
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
void Misc::AutoAccept()
{
    if (Globals::autoAccept)
    {
        //DWORD clientState = hazedumper::signatures::dwClientState;
        //DWORD gameState = mem.read<DWORD>(clientState + hazedumper::signatures::dwClientState_State);

        //if (gameState == 6) // 6 represents the "ACCEPT" state
        //{
        //    mem.write<int>(clientState + hazedumper::signatures::dwClientState_State, 0); // Set the state to 0 to accept the match
        //}
    }

}
Misc oMisc;



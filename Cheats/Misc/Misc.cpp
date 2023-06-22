#include "Misc.h"
#include "../../Valve/SDK/Const.h"
#include "../../Valve/SDK/SDK.h"
#include "../../Valve/SDK/CInput.h"

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

//void SetClan(const char* tag, const char* name) {
//    static auto pSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(((DWORD)mem.FindPattern("engine.dll", (PBYTE)"\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15\x00\x00\x00\x00\x6A\x24\x8B\xC8\x8B\x30", "xxxxxxxxx????xxxxxx")));
//    pSetClanTag(tag, name);
//}
//
//void Misc::Animated_Clantag() {
//    static int counter = 17;
//    static std::string clantag = "alaskaHOOK               ";
//    if (++counter > 25) {
//        std::rotate(clantag.begin(), clantag.begin() + 1, clantag.end());
//        SetClan(clantag.c_str(), clantag.c_str());
//        counter = 0;
//    }
//}




Misc oMisc;



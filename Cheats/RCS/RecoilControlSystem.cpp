#include "RecoilControlSystem.h"

#include "../../Valve/SDK/Vector3.h"
#include "../../Valve/SDK/Const.h"
#include "../../Valve/SDK/SDK.h"
#include "../../Valve/SDK/Vector3.h"


using namespace SDK;
using namespace std;
//Vec3* aimRecoilPunch;
//Vec3* oPunch[3]{ 0, 0, 0 };
//Vec3* viewAngles;

void RecoilControlSystem::InitRCS()
{
    //LocalPlayer = (DWORD)mem.read<DWORD>(g_client_base + hazedumper::signatures::dwLocalPlayer);
    //ClientState = (DWORD)mem.read<DWORD>(g_engine + hazedumper::signatures::dwClientState);
    //ShotsFired = (int*)mem.read<DWORD>(LocalPlayer + hazedumper::netvars::m_iShotsFired);
    //viewAngles = (Vec3*)(*(uintptr_t*)(g_engine + ClientState) = hazedumper::signatures::dwClientState_ViewAngles);
    //aimRecoilPunch = (Vec3*)(LocalPlayer + hazedumper::netvars::m_aimPunchAngle);
}


void RecoilControlSystem::RCS()
{
    //if (GetAsyncKeyState(VK_LBUTTON) && Globals::RCS_Enabled)
    

    //std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

RecoilControlSystem oRecoilControlSystem;
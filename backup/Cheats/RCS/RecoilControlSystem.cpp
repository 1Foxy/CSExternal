#include "RecoilControlSystem.h"
#include "../../Valve/SDK/Const.h"
#include "../../Valve/SDK/SDK.h"
#include "../../Valve/SDK/Vector3.h"

using namespace SDK;
using namespace std;

VVector3 RecoilControlSystem::ClampAngle(VVector3 angle)
{
    if (angle.x > 89.0f)
        angle.x = 89.0f;

    if (angle.x < -89.0f)
        angle.x = -89.0f;

    while (angle.y > 180)
        angle.y -= 360;

    while (angle.y < -180)
        angle.y += 360;

    angle.z = 0;

    return angle;
}

void RecoilControlSystem::ControlSpray()
{
    ShotsFired = mem.read<int>(LocalPlayer + hazedumper::netvars::m_iShotsFired);
    if (ShotsFired > 1)
    {
        Angle = mem.read<VVector3>(LocalPlayer + hazedumper::netvars::m_aimPunchAngle);
        AimPunch = OldAngle - Angle * 2.0f;
        AimPunch = ClampAngle(AimPunch);
        mem.write<VVector3>(ClientState + hazedumper::signatures::dwClientState_ViewAngles, AimPunch);
    }
    else
    {
        OldAngle = mem.read<VVector3>(ClientState + hazedumper::signatures::dwClientState_ViewAngles);
    }
}


void RecoilControlSystem::RCS()
{
    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
        ControlSpray();

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

RecoilControlSystem oRecoilControlSystem;
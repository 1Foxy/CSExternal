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

struct Vec3
{
    float x, y, z;
    Vec3 operator+(Vec3 d)
    {
		return { x + d.x, y + d.y, z + d.z };
	}
    Vec3 operator-(Vec3 d)
    {
		return { x - d.x, y - d.y, z - d.z };
	}
    Vec3 operator*(float d)
    {
        return { x * d, y * d, z * d };
    }
};

Vec3* aimRecoilPunch;
Vec3* oPunch{ 0, 0, 0 };


void RecoilControlSystem::ControlSpray()
{
    ShotsFired = (int*)mem.read<DWORD>(LocalPlayer + hazedumper::netvars::m_iShotsFired);
    aimRecoilPunch = (Vec3*)mem.read<DWORD>(LocalPlayer + hazedumper::netvars::m_aimPunchAngle); 

}

void RecoilControlSystem::RCS()
{
    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
        ControlSpray();

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

RecoilControlSystem oRecoilControlSystem;
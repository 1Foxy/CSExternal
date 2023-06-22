#pragma once
#include <Windows.h>

//
//struct Vec3
//{
//    float x, y, z;
//
//    Vec3 operator+(Vec3 d)
//    {
//        return { x + d.x, y + d.y, z + d.z };
//    }
//
//    Vec3 operator-(Vec3 d)
//    {
//        return { x - d.x, y - d.y, z - d.z };
//    }
//
//    Vec3 operator*(float d)
//    {
//        return { x * d, y * d, z * d };
//    }
//
//    void normalize()
//    {
//        while (y < -180) y = 360;
//        while (y > 180) y = 360;
//        if (x > 89) x = 89;
//        if (x < -89) x = -89;
//        z = 0;
//    }
//};
//
class RecoilControlSystem
{
public:
    void RCS();
    void InitRCS();

private:
    void ControlSpray();
//
//    //int* ShotsFired = 0;
//    //DWORD LocalPlayer = 0;
//    //DWORD ClientState = 0;
//    //Vector3* RecoilPunch[1]{0};
//    //Vector3* PunchAngle[1]{0};
}; extern RecoilControlSystem oRecoilControlSystem;
//

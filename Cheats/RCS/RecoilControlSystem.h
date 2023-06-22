#pragma once
#include <Windows.h>
#include "../../Valve/SDK/Vector3.h"

class RecoilControlSystem
{
public:
    void RCS();

private:
    VVector3 ClampAngle(VVector3 angle);
    void ControlSpray();
    int* ShotsFired = 0;
    VVector3 Angle;
    VVector3 AimPunch;
    VVector3 OldAngle;
    DWORD LocalPlayer = 0;
    DWORD ClientState = 0;
}; extern RecoilControlSystem oRecoilControlSystem;


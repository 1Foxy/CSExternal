#include "Aimbot.h"
#include "../../Valve/SDK/Const.h"
#include "../../Valve/SDK/SDK.h"
using namespace SDK;
using namespace std;

void Aimbot::aimbot()
{
    if (!Globals::Aimbot) return;

    DWORD localPlayer = mem.read<DWORD>(g_client_base + hazedumper::signatures::dwLocalPlayer);
    if (!localPlayer) return;

    int localPlayerTeam = mem.read<int>(localPlayer + hazedumper::netvars::m_iTeamNum);
    DWORD enginePointer = mem.read<DWORD>(g_engine + hazedumper::signatures::dwClientState);

    float oldDistX = 11111111.0f;
    float oldDistY = 11111111.0f;

    int target = 0;
    int targetHealth = 0;
    bool targetDormant = true;
    VVector3 targetPos, localPos, entityPos;
    float closestDistance = FLT_MAX;
    float distanceSq;
    VVector3 targetDistance;

    VVector3 localAngle = {
        mem.read<float>(enginePointer + hazedumper::signatures::dwClientState_ViewAngles),
        mem.read<float>(enginePointer + hazedumper::signatures::dwClientState_ViewAngles + 0x4),
        mem.read<float>(localPlayer + hazedumper::netvars::m_vecViewOffset + 0x8)
    };

    localPos.x = mem.read<float>(localPlayer + hazedumper::netvars::m_vecOrigin);
    localPos.y = mem.read<float>(localPlayer + hazedumper::netvars::m_vecOrigin + 0x4);
    localPos.z = mem.read<float>(localPlayer + hazedumper::netvars::m_vecOrigin + 0x8) + localAngle.z;

    for (int i = 1; i < 32; i++) {
        DWORD entity = mem.read<DWORD>(g_client_base + hazedumper::signatures::dwEntityList + i * 0x10);
        if (entity == 0) continue;

        int entityTeam = mem.read<int>(entity + hazedumper::netvars::m_iTeamNum);
        if (entityTeam == localPlayerTeam) continue;  // Skip friendly team members

        int entityHealth = mem.read<int>(entity + hazedumper::netvars::m_iHealth);
        bool entityDormant = mem.read<bool>(entity + hazedumper::signatures::m_bDormant);

        DWORD entityBones = mem.read<DWORD>(entity + hazedumper::netvars::m_dwBoneMatrix);

        entityPos.x = mem.read<float>(entityBones + 0x30 * oAimbot.selectedTargetBoneIndex + 0x0C);
        entityPos.y = mem.read<float>(entityBones + 0x30 * oAimbot.selectedTargetBoneIndex + 0x1C);
        entityPos.z = mem.read<float>(entityBones + 0x30 * oAimbot.selectedTargetBoneIndex + 0x2C);

        VVector3 tmp = localPos - entityPos;
        SDK::Vector2D angleVec = tmp.CalculateAngles(tmp);

        float distX = std::abs(angleVec.x - localAngle.x);
        if (distX > 89.0f) distX -= 360.0f;

        float distY = std::abs(angleVec.y - localAngle.y);
        if (distY > 180.0f) distY -= 360.0f;

        float fov = std::sqrt(distX * distX + distY * distY);
        if (fov > Globals::aFOV && Globals::UseFOV) continue;
        distanceSq = (localPos - entityPos).LengthSq();
        if (distanceSq < closestDistance) {
            DWORD curGlowIndex = mem.read<DWORD>(entity + hazedumper::netvars::m_iGlowIndex);
            DWORD glowObj = mem.read<DWORD>(g_client_base + hazedumper::signatures::dwGlowObjectManager);

            mem.write<float>(glowObj + curGlowIndex * 0x38 + 0x8, 0.0);
            mem.write<float>(glowObj + curGlowIndex * 0x38 + 0xC, 2.0);
            mem.write<float>(glowObj + curGlowIndex * 0x38 + 0x10, 0.0);

            closestDistance = distanceSq;
            targetDistance = tmp;
            oldDistX = distX;
            oldDistY = distY;
            target = entity;
            targetHealth = entityHealth;
            targetDormant = entityDormant;
            targetPos = entityPos;
        }
    }

    if (GetAsyncKeyState(0x12) < 0 && localPlayer != 0) {
        if (target != 0 && targetHealth > 0) {
            if (Globals::lockClosestPlayer) { // Check the boolean variable to lock onto the closest player
                target = 0; // Reset the target to lock onto the closest player
                closestDistance = FLT_MAX;
                oldDistX = 11111111.0;
                oldDistY = 11111111.0;
            }

            VVector3 tmp = localPos - targetPos;
            SDK::Vector2D angleVec = tmp.CalculateAngles(tmp);

            angleVec.Normalize(angleVec);

            if (Globals::autoShoot) {
                Sleep(Globals::autoShootDelay);
                mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, NULL);
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, NULL);
            }

            SDK::Vector2D currentAngles = {
                mem.read<float>(enginePointer + hazedumper::signatures::dwClientState_ViewAngles),
                mem.read<float>(enginePointer + hazedumper::signatures::dwClientState_ViewAngles + 0x4)
            };

            SDK::Vector2D newAngles = {
                lerp(currentAngles.x, angleVec.x, Globals::aimbotSmooth),
                lerp(currentAngles.y, angleVec.y, Globals::aimbotSmooth)
            };

            mem.write<float>(enginePointer + hazedumper::signatures::dwClientState_ViewAngles, newAngles.x);
            mem.write<float>(enginePointer + hazedumper::signatures::dwClientState_ViewAngles + 0x4, newAngles.y);

            Sleep(1);
        }
    }
}

Aimbot oAimbot;



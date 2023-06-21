#include "Aimbot.h"
#include "../../Valve/SDK/Const.h"
#include "../../Valve/SDK/SDK.h"
using namespace SDK;
using namespace std;

void Aimbot::aimbot()
{
    if (Globals::Aimbot) {
        DWORD localPlayer = mem.read<DWORD>(g_client_base + hazedumper::signatures::dwLocalPlayer);
        if (localPlayer) {
            int localPlayerTeam = mem.read<int>(localPlayer + hazedumper::netvars::m_iTeamNum);
            DWORD enginePointer = mem.read<DWORD>(g_engine + hazedumper::signatures::dwClientState);

            float oldDistX = 11111111.0;
            float oldDistY = 11111111.0;

            int target = 0;
            int targetHealth = 0;
            bool targetDormant = true;
            VVector3 targetPos, localPos;
            VVector3 localAngle, entityPos;
            float closestDistance = FLT_MAX;

            for (int i = 1; i < 32; i++) {
                DWORD entity = mem.read<DWORD>(g_client_base + hazedumper::signatures::dwEntityList + i * 0x10);
                if (entity == 0) {
                    continue;
                }

                int entityTeam = mem.read<int>(entity + hazedumper::netvars::m_iTeamNum);
                if (entityTeam == localPlayerTeam) {
                    continue;  // Skip friendly team members
                }

                int entityHealth = mem.read<int>(entity + hazedumper::netvars::m_iHealth);
                bool entityDormant = mem.read<bool>(entity + hazedumper::signatures::m_bDormant);

                localAngle.x = mem.read<float>(enginePointer + hazedumper::signatures::dwClientState_ViewAngles + 0x0);
                localAngle.y = mem.read<float>(enginePointer + hazedumper::signatures::dwClientState_ViewAngles + 0x4);
                localAngle.z = mem.read<float>(localPlayer + hazedumper::netvars::m_vecViewOffset + 0x8);
                localPos.x = mem.read<float>(localPlayer + hazedumper::netvars::m_vecOrigin + 0x0);
                localPos.y = mem.read<float>(localPlayer + hazedumper::netvars::m_vecOrigin + 0x4);
                localPos.z = mem.read<float>(localPlayer + hazedumper::netvars::m_vecOrigin + 0x8) + localAngle.z;

                DWORD entityBones = mem.read<DWORD>(entity + hazedumper::netvars::m_dwBoneMatrix);

                entityPos.x = mem.read<float>(entityBones + 0x30 * oAimbot.selectedTargetBoneIndex + 0x0C);
                entityPos.y = mem.read<float>(entityBones + 0x30 * oAimbot.selectedTargetBoneIndex + 0x1C);
                entityPos.z = mem.read<float>(entityBones + 0x30 * oAimbot.selectedTargetBoneIndex + 0x2C);

                VVector3 tmp = localPos - entityPos;
                SDK::Vector2D angleVec = tmp.CalculateAngles(tmp);

                float distX = angleVec.x - localAngle.x;
                if (distX < -89.0) {
                    distX += 360.0;
                }
                else if (distX > 89.0) {
                    distX -= 360.0;
                }
                if (distX < 0.0) {
                    distX = -distX;
                }
                float distY = angleVec.y - localAngle.y;
                if (distY < -180.0) {
                    distY += 360.0;
                }
                else if (distY > 180.0) {
                    distY -= 360.0;
                }
                if (distY < 0.0) {
                    distY = -distY;
                }

                bool entityCrouched = mem.read<int>(entity + hazedumper::netvars::m_fFlags) & (1 << 0);

                if (distX < (oldDistX - 0.25) && distY < (oldDistY - 0.25) && distX <= Globals::aFOV && distY <= Globals::aFOV && distX && entityCrouched && !entityDormant) {
                    float distanceSq = (localPos - entityPos).LengthSq();
                    if (distanceSq < closestDistance) {
                        DWORD curGlowIndex = mem.read<DWORD>(entity + hazedumper::netvars::m_iGlowIndex);
                        DWORD glowObj = mem.read<DWORD>(g_client_base + hazedumper::signatures::dwGlowObjectManager);
                        mem.write<float>(glowObj + curGlowIndex * 0x38 + 0x8, 0.0);
                        mem.write<float>(glowObj + curGlowIndex * 0x38 + 0xC, 2.0);
                        mem.write<float>(glowObj + curGlowIndex * 0x38 + 0x10, 0.0);

                        closestDistance = distanceSq;
                        oldDistX = distX;
                        oldDistY = distY;
                        target = entity;
                        targetHealth = entityHealth;
                        targetDormant = entityDormant;
                        targetPos = entityPos;
                    }
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


                    SDK::Vector2D currentAngles;
                    currentAngles.x = mem.read<float>(enginePointer + hazedumper::signatures::dwClientState_ViewAngles);
                    currentAngles.y = mem.read<float>(enginePointer + hazedumper::signatures::dwClientState_ViewAngles + 0x4);

                    SDK::Vector2D newAngles;
                    newAngles.x = lerp(currentAngles.x, angleVec.x, Globals::aimbotSmooth);
                    newAngles.y = lerp(currentAngles.y, angleVec.y, Globals::aimbotSmooth);

                    mem.write<float>(enginePointer + hazedumper::signatures::dwClientState_ViewAngles, newAngles.x);
                    mem.write<float>(enginePointer + hazedumper::signatures::dwClientState_ViewAngles + 0x4, newAngles.y);

                    Sleep(1);
                }
            }
        }
    }
}

Aimbot oAimbot;



#include "Aimbot.h"
#include "../../Valve/SDK/Const.h"
#include "../../Valve/SDK/SDK.h"
using namespace SDK;
using namespace std;
#include <algorithm>
// vector3
#include "../../Valve/SDK/Vector3.h"


struct PlayerInfo {
    DWORD entity;
    int health;
    bool dormant;
    bool inAirOrCrouching;
    VVector3 pos;
    float distanceSq;
};

std::vector<PlayerInfo> players;

void Aimbot::aimbot() {
    if (!Globals::Aimbot) return;
    players.clear();
    DWORD currentTarget = 0;

    // Cache some values
    int TargetFOV = Globals::aimbotFOV;
    localPlayer = mem.read<DWORD>(g_client_base + hazedumper::signatures::dwLocalPlayer);
    if (!localPlayer) return;

    int localPlayerTeam = mem.read<int>(localPlayer + hazedumper::netvars::m_iTeamNum);
    DWORD enginePointer = mem.read<DWORD>(g_engine + hazedumper::signatures::dwClientState);

    VVector3 localAngle = {
        mem.read<float>(enginePointer + hazedumper::signatures::dwClientState_ViewAngles),
        mem.read<float>(enginePointer + hazedumper::signatures::dwClientState_ViewAngles + 0x4),
        mem.read<float>(localPlayer + hazedumper::netvars::m_vecViewOffset + 0x8)
    };

    VVector3 localPos = {
        mem.read<float>(localPlayer + hazedumper::netvars::m_vecOrigin),
        mem.read<float>(localPlayer + hazedumper::netvars::m_vecOrigin + 0x4),
        mem.read<float>(localPlayer + hazedumper::netvars::m_vecOrigin + 0x8) + localAngle.z
    };

    // Fill players with all active entities
    for (int i = 1; i < 32; i++) {
        DWORD entity = mem.read<DWORD>(g_client_base + hazedumper::signatures::dwEntityList + i * 0x10);
        if (entity == 0) continue;

        int entityTeam = mem.read<int>(entity + hazedumper::netvars::m_iTeamNum);
        if (entityTeam == localPlayerTeam) continue;  // Skip friendly team members

        bool entityDormant = mem.read<bool>(entity + hazedumper::signatures::m_bDormant);
        int entityHealth = mem.read<int>(entity + hazedumper::netvars::m_iHealth);
        if (IsDead(entity) || entityDormant || entityHealth <= 0) continue;

        DWORD entityBones = mem.read<DWORD>(entity + hazedumper::netvars::m_dwBoneMatrix);

        VVector3 entityPos;
        entityPos.x = mem.read<float>(entityBones + 0x30 * oAimbot.selectedTargetBoneIndex + 0x0C);
        entityPos.y = mem.read<float>(entityBones + 0x30 * oAimbot.selectedTargetBoneIndex + 0x1C);
        entityPos.z = mem.read<float>(entityBones + 0x30 * oAimbot.selectedTargetBoneIndex + 0x2C);

        float distanceSq = (localPos - entityPos).LengthSq();
        bool inAirOrCrouching = IsCrouchingOrInAir(entity);


        players.push_back({ entity, entityHealth, entityDormant, inAirOrCrouching, entityPos, distanceSq });
    }

    // Filter by FOV
    if (Globals::UseFOV)
    {
        players.erase(std::remove_if(players.begin(), players.end(), [localAngle, localPos, TargetFOV](const PlayerInfo& player) {
            VVector3 tmp = localPos - player.pos;
            SDK::Vector2D angleVec = tmp.CalculateAngles(tmp);
            float distX = std::abs(angleVec.x - localAngle.x);
            if (distX > 89.0f) distX -= 360.0f;
            float distY = std::abs(angleVec.y - localAngle.y);
            if (distY > 180.0f) distY -= 360.0f;
            float fov = std::sqrt(distX * distX + distY * distY);
            return fov > TargetFOV;
        }), players.end());
    }

    // if players.lent is 0, return
    if (players.empty()) return;

    // Sorting by a combination of distance and angle to the current aim direction
    std::sort(players.begin(), players.end(), [localPos, localAngle](const PlayerInfo& a, const PlayerInfo& b) {
        float a_dist = a.distanceSq;
        float b_dist = b.distanceSq;
        // Getting the angles to the players
        float a_angle = (localPos - a.pos).CalculateAngles(localPos).Length();
        float b_angle = (localPos - b.pos).CalculateAngles(localPos).Length();
        // Comparing the distances multiplied by a function of the angle
        // Smaller angles increase the multiplier, making closer targets more desirable
        return a_dist * (1 + a_angle / 180.0f) < b_dist * (1 + b_angle / 180.0f);
    });


    // Check if its in the max lock distance
    if (Globals::UseMaxLockDistance)
        for (auto it = players.begin(); it != players.end();) {
            if (!MaxLockDistance(localPlayer, it->entity)) {
                it = players.erase(it);
            }
            else {
                ++it;
            }
        }

    if (players.empty()) return;

    // if not empty we will set current target to the first player
    currentTarget = players[0].entity;

    // If currentTarget is not valid, find a new target
    if (IsDead(currentTarget) || !MaxLockDistance(localPlayer, currentTarget)) {
        currentTarget = players[0].entity;
    }

    // Only proceed if the target is alive
    if (mem.read<int>(currentTarget + hazedumper::netvars::m_iHealth) > 0) {
        VVector3 targetPos;
        DWORD targetBones = mem.read<DWORD>(currentTarget + hazedumper::netvars::m_dwBoneMatrix);
        targetPos.x = mem.read<float>(targetBones + 0x30 * oAimbot.selectedTargetBoneIndex + 0x0C);
        targetPos.y = mem.read<float>(targetBones + 0x30 * oAimbot.selectedTargetBoneIndex + 0x1C);
        targetPos.z = mem.read<float>(targetBones + 0x30 * oAimbot.selectedTargetBoneIndex + 0x2C);

        if (GetAsyncKeyState(0x12) < 0 && localPlayer != 0) {
            VVector3 tmp = localPos - targetPos;
            SDK::Vector2D angleVec = tmp.CalculateAngles(tmp);

            angleVec.Normalize(angleVec);

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
        }
    }
}

bool Aimbot::MaxLockDistance(DWORD localPlayer, DWORD entity)
{
    // We'll assume that if the entity is within a certain range, it's visible.
    int maxVisibleDistance = Globals::MaxLockDistance;

    VVector3 localPlayerPos;
    localPlayerPos.x = mem.read<float>(localPlayer + hazedumper::netvars::m_vecOrigin);
    localPlayerPos.y = mem.read<float>(localPlayer + hazedumper::netvars::m_vecOrigin + 0x4);
    localPlayerPos.z = mem.read<float>(localPlayer + hazedumper::netvars::m_vecOrigin + 0x8);

    DWORD entityBones = mem.read<DWORD>(entity + hazedumper::netvars::m_dwBoneMatrix);

    VVector3 entityPos;
    entityPos.x = mem.read<float>(entityBones + 0x30 * oAimbot.selectedTargetBoneIndex + 0x0C);
    entityPos.y = mem.read<float>(entityBones + 0x30 * oAimbot.selectedTargetBoneIndex + 0x1C);
    entityPos.z = mem.read<float>(entityBones + 0x30 * oAimbot.selectedTargetBoneIndex + 0x2C);

    float distance = (localPlayerPos - entityPos).Length();

    return distance <= maxVisibleDistance;
}

bool Aimbot::IsDead(DWORD entity) {
    int entityHealth = mem.read<int>(entity + hazedumper::netvars::m_iHealth);
    return entityHealth <= 0;
}

bool Aimbot::IsCrouchingOrInAir(DWORD entity) {
    int entityFlags = mem.read<int>(entity + hazedumper::netvars::m_fFlags);
    return entityFlags == 263 || entityFlags == 256;
}

Aimbot oAimbot;
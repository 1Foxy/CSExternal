#include "Visuals.h"
#include "../../Header Files/globals.h"
#include "../../Header Files/Hazedumper.h"
#include "../../Valve/SDK/SDK.h"
#include "../../Valve/SDK/Const.h"
#include "../../Valve/SDK/CInput.h"

void Visuals::Noflash()
{
    auto local_player = SDK::get_local_player();
    if (!local_player)
        return;
    if (Globals::No_Flash) {
        if (!local_player)
        {
            return;
        }

        //// Assuming you have access to the flash duration variable
        float* flash_duration = reinterpret_cast<float*>(local_player + hazedumper::netvars::m_flFlashMaxAlpha);
        if (!flash_duration)
        {
            return;
        }

        float FlashTIme = local_player->GetFlashTime();
        if (FlashTIme > 0) {
            //add here if(Globals::LogFlashTime)
  /*          printf("Flash Time Left: %f\n", FlashTIme);*/
            local_player->SetFlashTime(0.0f);
        }

    }

}
void Visuals::Fov()
{
    uintptr_t localPlayer = mem.read<uintptr_t>(g_client_base + hazedumper::signatures::dwLocalPlayer);
    int flags = mem.read<int>(localPlayer + hazedumper::netvars::m_fFlags);


    if (Globals::FOV == 90)
        return;

    auto local_player = SDK::get_local_player();
    if (!local_player)
        return;

    if (local_player->isScoped())
        return;

    mem.write<int>(localPlayer + hazedumper::netvars::m_iDefaultFOV, Globals::FOV);
     
}

void Visuals::NoHands()
{
    uintptr_t localplayer = mem.read<uintptr_t>(g_client_base + hazedumper::signatures::dwLocalPlayer); //getlocalplayer
        if (Globals::No_Hands)
            mem.write<int>(localplayer + 0x258, 0); //n_ModelIndex 0x258
        else
            mem.write<int>(localplayer + 0x258, 339); //n_ModelIndex 0x258, 339 = CT MODEL INDEX
}

void Visuals::Glow() {
    if (Globals::Glow) {
        // glow
        const auto localPlayer = mem.read<std::uintptr_t>(g_client_base + hazedumper::signatures::dwLocalPlayer);
        const auto glowObjectManager = mem.read<std::uintptr_t>(g_client_base + hazedumper::signatures::dwGlowObjectManager);
        const auto localPlayerTeam = mem.read<int32_t>(localPlayer + hazedumper::netvars::m_iTeamNum);

        for (auto i = 1; i <= 32; ++i)
        {
            const auto entity = mem.read<std::uintptr_t>(g_client_base + hazedumper::signatures::dwEntityList + i * 0x10);

            if (!entity)
                continue;

            const auto entityTeam = mem.read<int32_t>(entity + hazedumper::netvars::m_iTeamNum);

            // Check if the entity is on our team and the GlowTeam flag is disabled
            if (entityTeam == localPlayerTeam && !Globals::Glow_Team)
                continue;

            const auto glowIndex = mem.read<int32_t>(entity + hazedumper::netvars::m_iGlowIndex);

            // do glow by writing each variable
            auto color = Color{ Globals::EnemyglowColorRed, Globals::EnemyglowColorGreen, Globals::EnemyglowColorBlue };
            mem.write<Color>(glowObjectManager + (glowIndex * 0x38) + 0x8, color);

            mem.write<bool>(glowObjectManager + (glowIndex * 0x38) + 0x28, true);
            mem.write<bool>(glowObjectManager + (glowIndex * 0x38) + 0x29, false);
        }

    }






  //  if (!Globals::Glow)
  //      return;

  //  const auto localPlayer = mem.read<std::uintptr_t>(g_client_base + hazedumper::signatures::dwLocalPlayer);
  //  const auto glowObjectManager = mem.read<std::uintptr_t>(g_client_base + hazedumper::signatures::dwGlowObjectManager);
  //  const auto localPlayerTeam = mem.read<int32_t>(localPlayer + hazedumper::netvars::m_iTeamNum);

  //  for (auto i = 1; i <= 32; ++i) {
  //      const auto entity = mem.read<std::uintptr_t>(g_client_base + hazedumper::signatures::dwEntityList + i * 0x10);

  //      if (!entity)
  //          continue;

  //      const auto entityTeam = mem.read<int32_t>(entity + hazedumper::netvars::m_iTeamNum);
  //      const auto classId = mem.read<int32_t>(entity + 0x117c8);


  ////      try {
  ////          mem.read<int32_t>(entity + 0x117c8);
  ////          printf("Entity class ID: %d\n", classId);

		////}
  ////      catch (...) {
  ////          printf ("Error reading entity class ID\n");
		////}

  //      // Check if the entity is on our team and the GlowTeam flag is disabled
  //      if (entityTeam == localPlayerTeam && !Globals::Glow_Team)
  //          continue;

  //      const auto glowIndex = mem.read<int32_t>(entity + hazedumper::netvars::m_iGlowIndex);
  //      Color color = Color{ 0, 0, 0 };

  //      //if (classId == 6) // Player
  //      //{
  //      //    if (entityTeam == localPlayerTeam)
  //      //        color = Color{ Globals::TeamglowColorRed, Globals::TeamglowColorGreen, Globals::TeamglowColorBlue }; // Teammates
  //      //    else
  //      //        color = Color{ Globals::EnemyglowColorRed, Globals::EnemyglowColorGreen, Globals::EnemyglowColorBlue }; // Enemies
  //      //}
  //      //else // Weapons and others
  //      //    color = Color{ Globals::weaponGlowColorRed, Globals::weaponGlowColorGreen, Globals::weaponGlowColorBlue };

  //      color = Color{ Globals::EnemyglowColorRed, Globals::EnemyglowColorGreen, Globals::EnemyglowColorBlue };
  //      // do glow by writing each variable
  //      mem.write<Color>(glowObjectManager + (glowIndex * 0x38) + 0x8, color);

  //      mem.write<bool>(glowObjectManager + (glowIndex * 0x38) + 0x24, true);
  //      mem.write<bool>(glowObjectManager + (glowIndex * 0x38) + 0x25, false);
  //  }
}


void Visuals::WeaponGlow()
 {
    //if (Globals::Glow_Weapons) {
    //    auto local_player = SDK::get_local_player();
    //    if (!local_player)
    //        return;

    //    D3DXVECTOR3 currentAngles = local_player->getLocalViewAngles();

    //    // Modify the view angles as desired
    //    currentAngles.x = 0.0f;  // Set the pitch to 90 degrees (up)
    //    currentAngles.y += 0.0f;  // Add 180 degrees to the yaw

    //    // Ensure that the yaw angle stays within the valid range of 0 to 360 degrees
    //    if (currentAngles.y > 360.0f)
    //        currentAngles.y -= 360.0f;
    //    else if (currentAngles.y < 0.0f)
    //        currentAngles.y += 360.0f;

    //    // Set the modified view angles
    //    local_player->setLocalViewAngles(currentAngles);



    //    //


    ////    //if ((pCmd->buttons & IN_JUMP) && !(pLocal->GetFlags() & (int)FL_ONGROUND))
    ////    //    pCmd->buttons &= ~IN_JUMP; //Release the JUMP button




    ////    //// Get the local player entity
    ////    //const auto localPlayer = mem.read<std::uintptr_t>(g_client_base + hazedumper::signatures::dwLocalPlayer);

    ////    //if (!localPlayer)
    ////    //    return;

    ////    //// Get the index of the active weapon
    ////    //const auto activeWeapon = mem.read<int32_t>(localPlayer + hazedumper::netvars::m_hActiveWeapon) & 0xFFF;

    ////    //// Get the entity pointer of the active weapon
    ////    //const auto weaponEntity = mem.read<std::uintptr_t>(g_client_base + hazedumper::signatures::dwEntityList + activeWeapon * 0x10);

    ////    //if (!weaponEntity)
    ////    //    return;

    ////    //// Check if the entity is a weapon (class ID 29) for the glow
    ////    //const auto classId = mem.read<int32_t>(localPlayer + hazedumper::Private::m_iClass);

    ////    //printf("engine.dll -> 0x%x\n", classId);

    ////    //if (classId == 29) {
    ////    //    // Get the glow index of the weapon
    ////    //    const auto glowIndex = mem.read<int32_t>(weaponEntity + hazedumper::netvars::m_iGlowIndex);

    ////    //    // Calculate the address of the glow object for the weapon
    ////    //    const auto glowObjectManager = mem.read<std::uintptr_t>(g_client_base + hazedumper::signatures::dwGlowObjectManager);
    ////    //    const auto glowObject = glowObjectManager + glowIndex * 0x38;

    ////    //    // Write the glow color directly
    ////    //    mem.write<Color>(glowObject + 0x8, color);

    ////    //    // Write the glow enable and disable values directly
    ////    //    mem.write<bool>(glowObject + 0x28, true);
    ////    //    mem.write<bool>(glowObject + 0x29, false);
    ////    //}
    //}
}



Visuals oVisuals;

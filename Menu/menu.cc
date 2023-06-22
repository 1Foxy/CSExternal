#include "menu.hh"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui.h"
#include "../font/icons.h"
#include "../Header Files/UI.h"
#include "../Header Files/common.h"

using namespace ImGui;


static void HelpMarker(const char* desc);
void CenterText(const char* text);

int selectedItemIndex = 0;
int selectedClanTagIndex = 0;
const char* items[] = { "Head", "Neck", "Chest", "Stomach", "Penis", "Feet" };

void ui::renderMenu() {
    if (!globals.active) return;

    ImGui::SetNextWindowPos(ImVec2(window_pos.x, window_pos.y), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(window_size.x, window_size.y));
    ImGui::SetNextWindowBgAlpha(1.0f);
    ImGui::Begin(window_title, &globals.active, window_flags);
    { 
      

        BeginChild("##buttons" , ImVec2(100,265));
        {
            SetCursorPosX(20);
            SetCursorPosY(5);
            if (Button("AIMBOT", ImVec2(60, 60))) globals.page = 0;
            SetCursorPosX(20);
            if (Button("VISUALS", ImVec2(60, 60))) globals.page = 1;
            SetCursorPosX(20);
            if (Button(("MISC"), ImVec2(60, 60))) globals.page = 2;
            SetCursorPosX(20);
            if (Button(("SETTINGS"), ImVec2(60, 60)))  globals.page = 3;
        }
        EndChild();
        SameLine();
        SetCursorPosX(110); 
        BeginChild("##pages", ImVec2(380, 400));
        {
            if (globals.page == 0)
            {
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
                BeginChild("##pages", ImVec2(155, 250), TRUE);
                {
                    ImGui::Checkbox("Aimbot", &Globals::Aimbot);
                    if (Globals::Aimbot)
                    { 
                    ImGui::SameLine();
                    if (ImGui::Combo(".", &selectedItemIndex, items, IM_ARRAYSIZE(items)))
                    {
                        switch (selectedItemIndex)
                        {
                        case 0:
                            oAimbot.selectedTargetBoneIndex = 8; // accurate head
                            break;
                        case 1:
                            oAimbot.selectedTargetBoneIndex = 7; // neck
                            break;
                        case 2:
                            oAimbot.selectedTargetBoneIndex = 6; //chest
                            break;
                        case 3:
                            oAimbot.selectedTargetBoneIndex = 4; //stomach
                            break;
                        case 4:
                            oAimbot.selectedTargetBoneIndex = 0; // penis
                            break;
                        case 5:
                            oAimbot.selectedTargetBoneIndex = 77; //feet // not very accurate (it changes with Model)
                            break;
                        }
                    }
                    ImGui::Checkbox("UseFOV", &Globals::UseFOV);
                    if (Globals::UseFOV)
                    ImGui::SliderInt("FOV", &Globals::aimbotFOV, 0, 140, "%d");
                    ImGui::SliderFloat("Smooth", &Globals::aimbotSmooth, 0.01f, 1.f, "%f");
                    ImGui::Checkbox("Use Max Lock Distance", &Globals::UseMaxLockDistance);
                    if (Globals::UseMaxLockDistance)
                    ImGui::SliderInt("Max Lock Distance", &Globals::MaxLockDistance, 500, 1000000, "%d");
                    ImGui::Checkbox("autoShoot", &Globals::autoShoot);
                    ImGui::SameLine();
                    ImGui::SliderInt("", &Globals::autoShootDelay, 0, 300, "%d");
                    ImGui::Checkbox(" ", &Globals::prediction);
                    ImGui::SameLine();
                    ImGui::SliderFloat("Prediction ", &Globals::predictionTime, 0.01f, 10.f, "%f");
                    }
                }
                EndChild();
                //BeginChild("##pagess", ImVec2(150, 250), TRUE);
                //if (Globals::Aimbot)
                //{

                //}
                //EndChild();
            }
            if (globals.page == 1)
            {
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
                BeginChild("##pages", ImVec2(150, 250), TRUE);
                {
                    ImGui::Checkbox("Glow", &Globals::Glow);
                    ImGui::Checkbox("Teammates", &Globals::Glow_Team);
                   // ImGui::Checkbox("Weapons", &Globals::Glow_Weapons);
                    ImGui::SliderInt("Camera FOV", &Globals::CameraFOV, 1, 140, "%d");
                    
           


                }
                EndChild();
                ImGui::SameLine();
                BeginChild("##pagess", ImVec2(150, 250), TRUE);
                {
                    ImGui::Checkbox("No Hands", &Globals::No_Hands);
                    ImGui::Checkbox("No Flash", &Globals::No_Flash);
                }
                EndChild();
            }
            if (globals.page == 2)
            {

                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
                BeginChild("##pages", ImVec2(150, 250), TRUE);
                {
                    ImGui::Checkbox("Bhop", &Globals::bhop);
                    ImGui::Checkbox("Autoaccept", &Globals::autoAccept);
                }
                EndChild();
                ImGui::SameLine();
                BeginChild("##pagess", ImVec2(150, 250), TRUE);
                {

                }
                EndChild();
            }
            if (globals.page == 3)
            {

            }
        }
        EndChild();
    }
    ImGui::End();
}

void ui::init(LPDIRECT3DDEVICE9 device) {
    dev = device;

    

	if (window_pos.x == 0) {
    	RECT screen_rect{};
		GetWindowRect(GetDesktopWindow(), &screen_rect);
		screen_res = ImVec2(float(screen_rect.right), float(screen_rect.bottom));
		window_pos = (screen_res - window_size) * 0.5f;

		// init images here
        
	}
}

static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void CenterText(const char* text)
{
    if (text == nullptr)
        return;

    ImGui::Spacing();
    ImGui::SameLine((ImGui::GetContentRegionAvail().x / 2) - (ImGui::CalcTextSize(text).x / 2));
    ImGui::Text(text);
    ImGui::Spacing();
}

void ui::MenuStyle() {
    ImGui::StyleColorsDark();
}
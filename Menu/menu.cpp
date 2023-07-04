#include "menu.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui.h"
#include "../font/icons.h"
#include "../Header Files/UI.h"
#include "../Header Files/common.h"

using namespace ImGui;


static void HelpMarker(const char* desc);
void CenterText(const char* text);

void ui::renderMenu() {
    if (!globals.active) return;

    MenuStyle();

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
        BeginChild("##pages", ImVec2(480, 400));
        {
            if (globals.page == 0)
            {
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
                BeginChild("##pages", ImVec2(155, 250), TRUE);
                {
                    ImGui::Checkbox("Aimbot", &Globals::Aimbot);
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

                    ImGui::Checkbox("FOV", &Globals::UseFOV);
                    ImGui::SameLine();
                    ImGui::SliderInt("      ", &Globals::aimbotFOV, 0, 140, "%d");

                    ImGui::SliderFloat("Smooth", &Globals::aimbotSmooth, 0.01f, 1.f, "%f");

                    ImGui::Checkbox("Distance", &Globals::UseMaxLockDistance);
                    if (!Globals::UseMaxLockDistance) { Globals::MaxLockDistance == 100000000; }
                    ImGui::SameLine();
                    ImGui::SliderInt("    ", &Globals::MaxLockDistance, 0, 500, "%d");

                    ImGui::Checkbox("autoShoot", &Globals::autoShoot);
                    ImGui::SameLine();
                    ImGui::SliderInt("", &Globals::autoShootDelay, 0, 300, "%d");
                    
                }
                EndChild();
                ImGui::SameLine();
                BeginChild("##pagesss", ImVec2(150, 250), TRUE);
                {             
                    ImGui::Checkbox("RCS", &Globals::RCS);
                    ImGui::SameLine();
                    ImGui::SliderFloat("   ", &Globals::rcsAmount, 0.0f, 100.0f);
                }
                EndChild();
                ImGui::SameLine();
                BeginChild("##pagesssss", ImVec2(150, 250), TRUE);
                {
                }
                EndChild();

            }
            if (globals.page == 1)
            {
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
                BeginChild("##pages", ImVec2(150, 250), TRUE);
                {
                    ImGui::Checkbox("Glow", &Globals::Glow);
                    ImGui::Checkbox("Teammates", &Globals::Glow_Team);
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
    ImGuiStyle& style = ImGui::GetStyle();

    style.ScrollbarSize = 1;
    style.GrabMinSize = 1;
    style.WindowBorderSize = 0;
    style.ChildBorderSize = 1;
    style.PopupBorderSize = 0;
    style.FrameBorderSize = 1;
    style.WindowRounding = 0;
    style.ChildRounding = 0;
    style.FrameRounding = 0;
    style.PopupRounding = 0;
    style.ScrollbarRounding = 0;
    style.GrabRounding = 0;

    ImGui::SetNextWindowPos(ImVec2(-10, -10));
    ImGui::SetNextWindowBgAlpha(0.1f);

    //Window Size
    ImGui::SetNextWindowSize({ 600.000000f, 300.000000f });
    ImGui::SetNextWindowPos(ImVec2((1920 - 840) * .5, (1080 - 500) * .5), ImGuiCond_Once);

    style.Colors[ImGuiCol_WindowBg] = ImColor(0, 0, 0, 255);
    style.Colors[ImGuiCol_Text] = ImColor(255, 0, 34, 200);
    style.Colors[ImGuiCol_Button] = ImColor(0, 0, 0, 255); 
    style.Colors[ImGuiCol_FrameBg] = ImColor(0, 0, 0, 255); 
    style.Colors[ImGuiCol_FrameBgHovered] = ImColor(0, 0, 0, 0);
    style.Colors[ImGuiCol_FrameBgActive] = ImColor(0, 0, 0, 0);

    style.Colors[ImGuiCol_TitleBgActive] = ImColor(0, 0, 0, 255);
    style.Colors[ImGuiCol_CheckMark] = ImColor(255, 0, 34, 255);

    style.Colors[ImGuiCol_SliderGrab] = ImColor(255, 0, 34, 255);
    style.Colors[ImGuiCol_SliderGrabActive] = ImColor(255, 0, 34, 255);

    style.Colors[ImGuiCol_ButtonHovered] = ImColor(0, 0, 0, 0);
    style.Colors[ImGuiCol_ButtonActive] = ImColor(0, 0, 0, 0);
}
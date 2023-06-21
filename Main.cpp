#include "Main.h"
#include "Header Files/globals.h"
#include "Header Files/memory.hpp"
#include "Valve/Engine.h"
#include "Menu/menu.hh"
#include "Header Files/common.h"
#include "Header Files/hazedumper.h"
#include "Valve/SDK/Color.h"
#include "Valve/SDK/CInput.h"

c_memory mem = c_memory("csgo.exe");

void loopFunction()
{
    while (true)
    {
        oMisc.Bunnyhop();
        oVisuals.Noflash();
        oVisuals.Fov();
        oAimbot.aimbot();
        oVisuals.NoHands();
        oVisuals.Glow();
        oMisc.AutoAccept();
   /*     oVisuals.WeaponGlow();*/

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

// Main code
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    if (!AttachConsole(ATTACH_PARENT_PROCESS))
    {
        // Create a new console if no console is attached
        AllocConsole();
        AttachConsole(GetCurrentProcessId());
        FILE* pFile = nullptr;
        freopen_s(&pFile, "CONOUT$", "w", stdout);
        freopen_s(&pFile, "CONOUT$", "w", stderr);
    }

    if (!mem.attach())
    {
        printf("waiting for csgo.exe...\n");

        while (!mem.attach())
            Sleep(500);
    }

    printf("Csgo detected!\n\n");
    printf("Process id -> %i\n", mem.data().th32ProcessID);

    if (Engine::Initialize() == false) {
        printf("Engine Failed\n");
        Sleep(500);
        return 0;
    }

    if (!Globals::KeepConsoleOpen) {
        // Hide console window
        HWND Stealth;
        AllocConsole();
        Stealth = FindWindowA("ConsoleWindowClass", NULL);
        ShowWindow(Stealth, 0);
    }

    std::thread loopThread(loopFunction);

    // Create application window
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, ui::window_title, NULL };
    RegisterClassEx(&wc);
    main_hwnd = CreateWindow(wc.lpszClassName, ui::window_title, WS_POPUP, 0, 0, 5, 5, NULL, NULL, wc.hInstance, NULL);

    // Initialize Direct3D
    if (!CreateDeviceD3D(main_hwnd)) {
        CleanupDeviceD3D();
        UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ShowWindow(main_hwnd, SW_HIDE);
    UpdateWindow(main_hwnd);

    // Setup Dear ImGui context
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    io.IniFilename = nullptr; //crutial for not leaving the imgui.ini file
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
     
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(main_hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
    ImFontConfig icons_config;

    ImFontConfig CustomFont;
    CustomFont.FontDataOwnedByAtlas = false;

    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.OversampleH = 3;
    icons_config.OversampleV = 3;
    io.Fonts->AddFontDefault();

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {
            static int init = false;
            if (!init) {
                ui::init(g_pd3dDevice);
                init = true;
            }
            else {
                ui::renderMenu();
            }
        }
        ImGui::EndFrame();

        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }

        // Update and Render additional Platform Windows
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
            ResetDevice();
        }
        if (!globals.active) {
            msg.message = WM_QUIT;
        }
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    

    CleanupDeviceD3D();
    DestroyWindow(main_hwnd);
    UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
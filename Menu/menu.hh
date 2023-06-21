#pragma once
#include <d3d9.h>
#include "../imgui/imgui.h"
#include "../Header Files/globals.h"

namespace ui {
	void init(LPDIRECT3DDEVICE9);
	void renderMenu();
	void MenuStyle();
}

namespace ui {
	inline LPDIRECT3DDEVICE9 dev;
}

namespace ui {
	const char window_title[255] = ".";
	inline ImVec2 screen_res{ 000, 000 };
	inline ImVec2 window_pos{ 0, 0 };
	inline ImVec2 window_size{ 500, 300 };
	inline DWORD  window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
}
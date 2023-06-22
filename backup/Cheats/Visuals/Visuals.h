#pragma once
#include "../../Valve/SDK/Color.h"
// globals
#include "../../Header Files/globals.h"

__declspec(align(16)) struct Color
{
	constexpr Color(const float r, const float g, const float b, const float a = 1.f) noexcept :
		r(r), g(g), b(b), a(a) { }

	float r, g, b, a;
};

class Visuals
{
public:
	void Noflash();
	void Fov();
	void NoHands();
	void Glow();
	void WeaponGlow();
};

extern Visuals oVisuals;

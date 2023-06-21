#pragma once
#include "../../Valve/SDK/Color.h"

__declspec(align(16)) struct Color
{
	constexpr Color(const float r, const float g, const float b, const float a = 1.f) noexcept :
		r(r), g(g), b(b), a(a) { }

	float r, g, b, a;
};

constexpr const auto color = Color{ 1.f, 0.f, 1.f };

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

#pragma once
#include <unordered_map>
#include <thread>
#include <Windows.h>

namespace Globals {

	//aimbot
	inline bool Aimbot = false;
	inline bool autoShoot = false;
	inline bool lockClosestPlayer = true;
	inline bool prediction = false;
	inline bool RCS = true;

	inline int autoShootDelay = 50;
	inline float aimbotSmooth = 1.f;
	inline int aimbotKey = 1;
	inline int aFOV = 5;
	inline float predictionTime = 0.f;

	//visuals
	inline bool Glow = false;
	inline bool Glow_Team = false;
	inline bool Glow_Weapons = false;
	inline bool No_Hands = false;
	inline bool No_Flash = false;

	inline bool UseFOV = true;
	inline int FOV = 90;

	//misc
	inline bool autoAccept = false;
	inline bool bhop = false;

	//global
	inline bool KeepConsoleOpen = true;

	inline float EnemyglowColorRed = 1.f;
	inline float EnemyglowColorGreen = 0.f;
	inline float EnemyglowColorBlue = 1.f;

	inline float weaponGlowColorRed = 1.f;
	inline float weaponGlowColorGreen = 0.f;
	inline float weaponGlowColorBlue = 1.f;

	inline float TeamglowColorRed = 0.f;
	inline float TeamglowColorGreen = 1.f;
	inline float TeamglowColorBlue = 0.f;

}

class c_globals {
public:
	bool active = true;

	int page = 0;
	int subPage = 0;
};

class color_settings
{
public:
	float all[4] = { 0.20f, 0.20f, 0.20f, 255 };
};

inline c_globals globals;

namespace mouse
{
	void Move(int x,int y, int s);
	void click(int holdTime);
}

namespace keyboard
{
	void pressKey(int key, int holdTime);
}

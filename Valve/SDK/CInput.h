#pragma once

#include "CRC.h"
#include "ButtonCode.h"
#include "Vector3.h"
#include <cstddef>
#include <wtypes.h>
#include <rpcndr.h>


//#include "SDK.h"

#define VirtualFn( cast ) typedef cast( __thiscall* OriginalFn )

template< typename Function > Function GetMethod(PVOID Base, DWORD Index)
{
	PDWORD* VTablePointer = (PDWORD*)Base;
	PDWORD VTableFunctionBase = *VTablePointer;
	DWORD dwAddress = VTableFunctionBase[Index];
	return (Function)(dwAddress);
}



namespace SDK
{
#define IN_ATTACK		(1 << 0)
#define IN_JUMP			(1 << 1)
#define IN_DUCK			(1 << 2)
#define IN_FORWARD		(1 << 3)
#define IN_BACK			(1 << 4)
#define IN_USE			(1 << 5)
#define IN_CANCEL		(1 << 6)
#define IN_LEFT			(1 << 7)
#define IN_RIGHT		(1 << 8)
#define IN_MOVELEFT		(1 << 9)
#define IN_MOVERIGHT	(1 << 10)
#define IN_ATTACK2		(1 << 11)
#define IN_RUN			(1 << 12)
#define IN_RELOAD		(1 << 13)
#define IN_ALT1			(1 << 14)
#define IN_ALT2			(1 << 15)
#define IN_SCORE		(1 << 16)
#define IN_SPEED		(1 << 17)
#define IN_WALK			(1 << 18)
#define IN_ZOOM			(1 << 19)
#define IN_WEAPON1		(1 << 20)
#define IN_WEAPON2		(1 << 21)
#define IN_BULLRUSH		(1 << 22)
#define IN_GRENADE1		(1 << 23)
#define IN_GRENADE2		(1 << 24)
#define IN_LOOKSPIN		(1 << 25)
#define MULTIPLAYER_BACKUP 150

	class bf_read;
	class bf_write;

	class CUserCmd {
	public:
		virtual ~CUserCmd() {};

		CRC32_t GetChecksum(void) const
		{
			CRC32_t crc;
			CRC32_Init(&crc);

			CRC32_ProcessBuffer(&crc, &command_number, sizeof(command_number));
			CRC32_ProcessBuffer(&crc, &tick_count, sizeof(tick_count));
			CRC32_ProcessBuffer(&crc, &viewangles, sizeof(viewangles));
			CRC32_ProcessBuffer(&crc, &aimdirection, sizeof(aimdirection));
			CRC32_ProcessBuffer(&crc, &Move, sizeof(Move));
			CRC32_ProcessBuffer(&crc, &buttons, sizeof(buttons));
			CRC32_ProcessBuffer(&crc, &impulse, sizeof(impulse));
			CRC32_ProcessBuffer(&crc, &weaponselect, sizeof(weaponselect));
			CRC32_ProcessBuffer(&crc, &weaponsubtype, sizeof(weaponsubtype));
			CRC32_ProcessBuffer(&crc, &random_seed, sizeof(random_seed));
			CRC32_ProcessBuffer(&crc, &mousedx, sizeof(mousedx));
			CRC32_ProcessBuffer(&crc, &mousedy, sizeof(mousedy));

			CRC32_Final(&crc);
			return crc;
		}

		int       command_number;     // 0x04
		int       tick_count;         // 0x08
		QAngle    viewangles;         // 0x0C
		SDK::Vector3    aimdirection;       // 0x18
		SDK::Vector3    Move;
		int       buttons;            // 0x30
		byte      impulse;            // 0x34
		int       weaponselect;       // 0x38
		int       weaponsubtype;      // 0x3C
		int       random_seed;        // 0x40
		short     mousedx;            // 0x44
		short     mousedy;            // 0x46
		bool      hasbeenpredicted;   // 0x48
		char      pad_0x4C[0x18];     // 0x4C
	};


	class CVerifiedUserCmd
	{
	public:
		CUserCmd  m_cmd;
		CRC32_t   m_crc;
	};

	//27th July 2019
	class CInput {
	public:
		char pad_0x00[0x8];
		bool m_fTrackIRAvailable;
		bool m_fMouseInitialized;
		bool m_fMouseActive;
		bool m_fJoystickAdvancedInit;
		char pad_0x08[0x2C];
		void* m_pKeys;
		char pad1[0x10];
		float m_flKeyboardSampleTime;
		char pad2[0x58];
		bool m_fCameraInterceptingMouse;
		bool m_fCameraInThirdPerson;
		bool m_fCameraMovingWithMouse;
		//Vector m_vecCameraOffset;
		bool m_fCameraDistanceMove;
		int m_nCameraOldX;
		int m_nCameraOldY;
		int m_nCameraX;
		int m_nCameraY;
		bool m_CameraIsOrthographic;
	/*	Vector m_angPreviousViewAngles;
		Vector m_angPreviousViewAnglesTilt;*/
		float m_flLastForwardMove; // 0xEC
		int m_nClearInputState;
		CUserCmd* m_pCommands; // 0xF4
		CVerifiedUserCmd* m_pVerifiedCommands;

		inline CUserCmd& get_user_cmd(int sequence_number) const {
			return m_pCommands[sequence_number % MULTIPLAYER_BACKUP];
		}
		inline CVerifiedUserCmd& get_verified_user_cmd(int sequence_number) const {
			return m_pVerifiedCommands[sequence_number % MULTIPLAYER_BACKUP];
		}
	};

	class IInputSystem
	{
	public:
		void EnableInput(bool bEnable)
		{
			VirtualFn(void*)(PVOID, bool);
			GetMethod< OriginalFn >(this, 11)(this, bEnable);
		}

		bool IsButtonDown(ButtonCode_t code)
		{
			VirtualFn(bool)(PVOID, ButtonCode_t);
			return GetMethod< OriginalFn >(this, 15)(this, code);
		}

		void ResetInputState()
		{
			VirtualFn(void)(PVOID);
			GetMethod< OriginalFn >(this, 39)(this);
		}

		ButtonCode_t VirtualKeyToButtonCode(int nVirtualKey)
		{
			VirtualFn(ButtonCode_t)(PVOID, int);
			return GetMethod< OriginalFn >(this, 44)(this, nVirtualKey);
		}

		int ButtonCodeToVirtualKey(ButtonCode_t code)
		{
			VirtualFn(int)(PVOID, ButtonCode_t);
			return GetMethod< OriginalFn >(this, 45)(this, code);
		}

		void GetCursorPosition(int* m_pX, int* m_pY)
		{
			VirtualFn(void)(PVOID, int*, int*);
			GetMethod< OriginalFn >(this, 56)(this, m_pX, m_pY);
		}
	};
}
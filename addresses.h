#pragma once
#include "platform.h"
#include "stdint.h"
#include "utils/module.h"

namespace modules
{
	inline CModule *engine;
	inline CModule *server;
}

typedef CUtlStringToken WorldGroupId_t;

class CBasePlayerController;

namespace addresses
{
	void Initialize();
	inline void(FASTCALL *ClientPrint)(CBasePlayerController *player, int msg_dest, const char *msg_name, void *param1, void *param2, void *param3, void *param4);
	inline void(FASTCALL* CCSPlayerController_SwitchTeam)(CCSPlayerController* pController, uint32 team);
}

namespace sigs
{
#ifdef _WIN32
	inline const byte *ClientPrint = (byte *)"\x48\x85\xC9\x0F\x84\x2A\x2A\x2A\x2A\x48\x89\x5C\x24\x2A\x55";
	inline const byte *CCSPlayerController_SwitchTeam = (byte *)"\x40\x56\x57\x48\x81\xEC\x2A\x2A\x2A\x2A\x48\x8B\xF9\x8B\xF2\x8B\xCA";
#else
	inline const byte *ClientPrint = (byte *)"\x55\x48\x89\xE5\x41\x57\x49\x89\xCF\x41\x56\x49\x89\xD6\x41\x55\x41\x89\xF5\x41\x54\x4C\x8D\xA5\xA0\xFE\xFF\xFF";
	inline const byte *CCSPlayerController_SwitchTeam = (byte *)"\x55\x48\x89\xE5\x41\x55\x49\x89\xFD\x89\xF7";
#endif
}
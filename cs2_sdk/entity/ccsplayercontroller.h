#pragma once

#include "cbaseplayercontroller.h"
#include "services.h"
#include "ccsplayerpawn.h"
#include "addresses.h"

class CCSPlayerController : public CBasePlayerController
{
public:
	DECLARE_SCHEMA_CLASS(CCSPlayerController);

	SCHEMA_FIELD(CCSPlayerController_InGameMoneyServices*, m_pInGameMoneyServices)
	SCHEMA_FIELD(CCSPlayerController_InventoryServices*, m_pInventoryServices)
	SCHEMA_FIELD(CHandle< CCSPlayerPawn >, m_hPlayerPawn)
	SCHEMA_FIELD(bool, m_bPawnIsAlive)

	static CCSPlayerController* FromSlot(CPlayerSlot slot)
	{
		return (CCSPlayerController*)g_pEntitySystem->GetEntityInstance(CEntityIndex(slot.Get() + 1));
	}
	
	int Get_Team()
	{
		return this->m_iTeamNum();
	}
	
	void ChangeTeam(int iTeam)
	{
		CALL_VIRTUAL(void, WIN_LINUX(100, 99), this, iTeam);
	}
	
	void SwitchTeam(int iTeam)
	{
		if (!IsController())
			return;

		if (iTeam == CS_TEAM_SPECTATOR)
			ChangeTeam(iTeam);
		else
			addresses::CCSPlayerController_SwitchTeam(this, iTeam);
	}
	
	void ClientPrint(int msg_dest, const char *msg, ...)
	{
		char buf[1024] = {};
		va_list args;
		va_start(args, msg);
		vsnprintf(buf, sizeof(buf), msg, args);
		va_end(args);
		addresses::ClientPrint(this, msg_dest, buf, nullptr, nullptr, nullptr, nullptr);
	}
	
	bool IsAdmin()
	{
		return false;
	}
};

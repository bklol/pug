#pragma once

#include "ehandle.h"
#include "services.h"

class CCSPlayerController;

class CBasePlayerPawn : public CBaseEntity
{
public:
	DECLARE_SCHEMA_CLASS(CBasePlayerPawn);

	SCHEMA_FIELD(CPlayer_MovementServices*, m_pMovementServices)
	SCHEMA_FIELD(CPlayer_WeaponServices*, m_pWeaponServices)
	SCHEMA_FIELD(CPlayer_ObserverServices*, m_pObserverServices)
	SCHEMA_FIELD(CCSPlayer_ItemServices*, m_pItemServices)
	SCHEMA_FIELD(CHandle< CCSPlayerController >, m_hController)
	CCSPlayerController *GetController() { return m_hController().Get(); }
};
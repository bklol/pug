#pragma once

#include "../schema.h"
#include "globaltypes.h"
#include "mathlib/vector.h"
#include "ehandle.h"
#include <entity2/entityidentity.h>
#include "cbaseplayerweaponvdata.h"

CGlobalVars* GetGameGlobals();

class CBasePlayerPawn;

class CGameSceneNode
{
public:
	DECLARE_SCHEMA_CLASS_INLINE(CGameSceneNode)
};

class CBodyComponent
{
public:
	DECLARE_SCHEMA_CLASS_INLINE(CBodyComponent)

	SCHEMA_FIELD(CGameSceneNode*, m_pSceneNode)
};

class CBaseEntity : public CEntityInstance
{
public:
	DECLARE_SCHEMA_CLASS(CBaseEntity)
	SCHEMA_FIELD(CBitVec<64>, m_isSteadyState)
	SCHEMA_FIELD(float, m_lastNetworkChange)
	SCHEMA_FIELD_POINTER(CNetworkTransmitComponent, m_NetworkTransmitComponent)
	SCHEMA_FIELD(int, m_iHealth)
	SCHEMA_FIELD(int, m_iTeamNum)
	SCHEMA_FIELD(LifeState_t, m_lifeState)
	SCHEMA_FIELD(Vector, m_vecBaseVelocity)
	SCHEMA_FIELD(CBodyComponent*, m_CBodyComponent)
	SCHEMA_FIELD(MoveCollide_t, m_MoveCollide)
	SCHEMA_FIELD(CHandle< CBasePlayerPawn >, m_hOwnerEntity)
	bool IsController()
	{
		return CALL_VIRTUAL(bool, WIN_LINUX(162, 161), this);
	}
	int entindex() { return m_pEntity->m_EHandle.GetEntryIndex(); }
};

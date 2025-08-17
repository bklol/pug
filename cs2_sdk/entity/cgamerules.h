#pragma once
#include "ehandle.h"
#include "globaltypes.h"
#include "cbaseentity.h"

class CGameRules
{
public:
	DECLARE_SCHEMA_CLASS(CGameRules)
};

class CCSGameRules : public CGameRules
{
public:
	DECLARE_SCHEMA_CLASS(CCSGameRules)

	SCHEMA_FIELD(float, m_fMatchStartTime)
	SCHEMA_FIELD(float, m_flGameStartTime)
	SCHEMA_FIELD(int, m_totalRoundsPlayed)
	SCHEMA_FIELD(GameTime_t, m_fRoundStartTime)
	SCHEMA_FIELD(GameTime_t, m_flRestartRoundTime)
	SCHEMA_FIELD_POINTER(int, m_nEndMatchMapGroupVoteOptions)
	SCHEMA_FIELD(int, m_nEndMatchMapVoteWinner)
	SCHEMA_FIELD(int, m_iRoundTime)
};

class CCSGameRulesProxy : public CBaseEntity
{
public:
	DECLARE_SCHEMA_CLASS(CCSGameRulesProxy)

	SCHEMA_FIELD(CCSGameRules *, m_pGameRules)
};
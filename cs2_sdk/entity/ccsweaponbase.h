#pragma once

#include "schema.h"
#include <utlvector.h>
#include "ccsplayerpawn.h"
#include "cbaseentity.h"

class CEconEntity : public CBaseEntity
{
public:
	DECLARE_SCHEMA_CLASS(CEconEntity);
	SCHEMA_FIELD(uint32_t, m_iOldOwnerClass)
};

class CCSWeaponBase : public CEconEntity
{
public:
	DECLARE_SCHEMA_CLASS(CCSWeaponBase);
	SCHEMA_FIELD(uint32_t, m_nViewModelIndex)
	SCHEMA_FIELD(CHandle< CCSPlayerPawn >, m_hPrevOwner)
	SCHEMA_FIELD(bool, m_bWasOwnedByCT)
	SCHEMA_FIELD(bool, m_bWasOwnedByTerrorist)
	CBasePlayerPawn *GetOwnerEntity()
	{
		return this->m_hOwnerEntity();
	}
};
#pragma once

#include "ccsweaponbase.h"

class CBasePlayerWeapon : public CEconEntity
{
public:
	DECLARE_SCHEMA_CLASS(CBasePlayerWeapon);
	SCHEMA_FIELD(int32_t, m_iClip1)
	SCHEMA_FIELD(int32_t, m_iClip2)
	SCHEMA_FIELD(int32_t, m_pReserveAmmo)
};
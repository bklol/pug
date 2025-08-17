#pragma once
#include <platform.h>
#include "ehandle.h"
#include "../schema.h"
#include "cbaseviewmodel.h"
#include "ccsweaponbase.h"
#include "ccsplayerpawn.h"

enum class ObserverMode_t : uint32_t
{
	OBS_MODE_NONE = 0x0,
	OBS_MODE_FIXED = 0x1,
	OBS_MODE_IN_EYE = 0x2,
	OBS_MODE_CHASE = 0x3,
	OBS_MODE_ROAMING = 0x4,
	OBS_MODE_DIRECTED = 0x5,
	NUM_OBSERVER_MODES = 0x6,
};

class CPlayerPawnComponent
{
public:
	DECLARE_SCHEMA_CLASS(CPlayerPawnComponent);

	SCHEMA_FIELD(void*, __m_pChainEntity)
};

class CPlayer_ObserverServices : public CPlayerPawnComponent
{
public:
	DECLARE_SCHEMA_CLASS(CPlayer_ObserverServices);
	SCHEMA_FIELD(uint8_t, m_iObserverMode)
	SCHEMA_FIELD(ObserverMode_t, m_iObserverLastMode)
	SCHEMA_FIELD(bool, m_bForcedObserverMode)
};

class CPlayer_MovementServices : public CPlayerPawnComponent
{
public:
	DECLARE_SCHEMA_CLASS(CPlayer_MovementServices);
};

class CCSPlayerController_InGameMoneyServices : public CPlayerPawnComponent
{
public:
	DECLARE_SCHEMA_CLASS(CCSPlayerController_InGameMoneyServices);

    SCHEMA_FIELD(int, m_iAccount)
};

class CCSPlayerController_InventoryServices : public CPlayerPawnComponent
{
public:
	DECLARE_SCHEMA_CLASS(CCSPlayerController_InventoryServices);
};

class CPlayer_WeaponServices : public CPlayerPawnComponent
{
public:
	DECLARE_SCHEMA_CLASS(CPlayer_WeaponServices);

	SCHEMA_FIELD_POINTER(CUtlVector<CHandle<CBasePlayerWeapon>>, m_hMyWeapons)
	SCHEMA_FIELD(CHandle<CBasePlayerWeapon>, m_hActiveWeapon)
};

class CCSPlayer_ViewModelServices
{
public:
	DECLARE_SCHEMA_CLASS(CCSPlayer_ViewModelServices);
	SCHEMA_FIELD(CHandle< CBaseViewModel >, m_hViewModel)
};

class CCSPlayer_ItemServices : public CPlayerPawnComponent
{
public:
	DECLARE_SCHEMA_CLASS(CCSPlayer_ItemServices);
virtual ~CCSPlayer_ItemServices() = 0;
private:
	virtual void unk_01() = 0;
	virtual void unk_02() = 0;
	virtual void unk_03() = 0;
	virtual void unk_04() = 0;
	virtual void unk_05() = 0;
	virtual void unk_06() = 0;
	virtual void unk_07() = 0;
	virtual void unk_08() = 0;
	virtual void unk_09() = 0;
	virtual void unk_10() = 0;
	virtual void unk_11() = 0;
	virtual void unk_12() = 0;
	virtual void unk_13() = 0;
	virtual void unk_14() = 0;
	virtual void unk_15() = 0;
	virtual void unk_16() = 0;
	virtual CBaseEntity* _GiveNamedItem(const char* pchName) = 0;
public:
	virtual bool GiveNamedItemBool(const char* pchName) = 0;
	virtual CBaseEntity* GiveNamedItem(const char* pchName) = 0;
	virtual void DropPlayerWeapon(CBasePlayerWeapon* weapon) = 0;
	virtual void StripPlayerWeapons() = 0;
};
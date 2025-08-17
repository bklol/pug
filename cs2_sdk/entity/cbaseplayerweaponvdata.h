#pragma once
#include "../schema.h"
#include "cbaseentity.h"
#include "mathlib/vector.h"

enum class CSWeaponCategory : uint32_t
{
	WEAPONCATEGORY_OTHER = 0x0,
	WEAPONCATEGORY_MELEE = 0x1,
	WEAPONCATEGORY_SECONDARY = 0x2,
	WEAPONCATEGORY_SMG = 0x3,
	WEAPONCATEGORY_RIFLE = 0x4,
	WEAPONCATEGORY_HEAVY = 0x5,
	WEAPONCATEGORY_COUNT = 0x6,
};

class CFiringModeFloat
{
public:
	float m_flValues[2]; // 0x0	
};

class CFiringModeInt
{
public:
	int32_t m_nValues[2]; // 0x0	
};

enum class ItemFlagTypes_t : uint8_t
{
	ITEM_FLAG_NONE = 0x0,
	ITEM_FLAG_CAN_SELECT_WITHOUT_AMMO = 0x1,
	ITEM_FLAG_NOAUTORELOAD = 0x2,
	ITEM_FLAG_NOAUTOSWITCHEMPTY = 0x4,
	ITEM_FLAG_LIMITINWORLD = 0x8,
	ITEM_FLAG_EXHAUSTIBLE = 0x10,
	ITEM_FLAG_DOHITLOCATIONDMG = 0x20,
	ITEM_FLAG_NOAMMOPICKUPS = 0x40,
	ITEM_FLAG_NOITEMPICKUP = 0x80,
};

enum class CSWeaponSilencerType : uint32_t
{
	WEAPONSILENCER_NONE = 0x0,
	WEAPONSILENCER_DETACHABLE = 0x1,
	WEAPONSILENCER_INTEGRATED = 0x2,
};

enum class CSWeaponType : uint32_t
{
	WEAPONTYPE_KNIFE = 0x0,
	WEAPONTYPE_PISTOL = 0x1,
	WEAPONTYPE_SUBMACHINEGUN = 0x2,
	WEAPONTYPE_RIFLE = 0x3,
	WEAPONTYPE_SHOTGUN = 0x4,
	WEAPONTYPE_SNIPER_RIFLE = 0x5,
	WEAPONTYPE_MACHINEGUN = 0x6,
	WEAPONTYPE_C4 = 0x7,
	WEAPONTYPE_TASER = 0x8,
	WEAPONTYPE_GRENADE = 0x9,
	WEAPONTYPE_EQUIPMENT = 0xa,
	WEAPONTYPE_STACKABLEITEM = 0xb,
	WEAPONTYPE_FISTS = 0xc,
	WEAPONTYPE_BREACHCHARGE = 0xd,
	WEAPONTYPE_BUMPMINE = 0xe,
	WEAPONTYPE_TABLET = 0xf,
	WEAPONTYPE_MELEE = 0x10,
	WEAPONTYPE_SHIELD = 0x11,
	WEAPONTYPE_ZONE_REPULSOR = 0x12,
	WEAPONTYPE_UNKNOWN = 0x13,
};


enum class gear_slot_t : uint32_t
{
	GEAR_SLOT_INVALID = 0xFFFFFFFF,
	GEAR_SLOT_RIFLE = 0x0,
	GEAR_SLOT_PISTOL = 0x1,
	GEAR_SLOT_KNIFE = 0x2,
	GEAR_SLOT_GRENADES = 0x3,
	GEAR_SLOT_C4 = 0x4,
	GEAR_SLOT_RESERVED_SLOT6 = 0x5,
	GEAR_SLOT_RESERVED_SLOT7 = 0x6,
	GEAR_SLOT_RESERVED_SLOT8 = 0x7,
	GEAR_SLOT_RESERVED_SLOT9 = 0x8,
	GEAR_SLOT_RESERVED_SLOT10 = 0x9,
	GEAR_SLOT_RESERVED_SLOT11 = 0xa,
	GEAR_SLOT_BOOSTS = 0xb,
	GEAR_SLOT_UTILITY = 0xc,
	GEAR_SLOT_COUNT = 0xd,
	GEAR_SLOT_FIRST = 0x0,
	GEAR_SLOT_LAST = 0xc,
};

enum class loadout_slot_t : uint64_t
{
	LOADOUT_SLOT_PROMOTED = 0xfffffffffffffffe,
	LOADOUT_SLOT_INVALID = 0xffffffffffffffff,
	LOADOUT_SLOT_MELEE = 0x0,
	LOADOUT_SLOT_C4 = 0x1,
	LOADOUT_SLOT_FIRST_AUTO_BUY_WEAPON = 0x0,
	LOADOUT_SLOT_LAST_AUTO_BUY_WEAPON = 0x1,
	LOADOUT_SLOT_SECONDARY0 = 0x2,
	LOADOUT_SLOT_SECONDARY1 = 0x3,
	LOADOUT_SLOT_SECONDARY2 = 0x4,
	LOADOUT_SLOT_SECONDARY3 = 0x5,
	LOADOUT_SLOT_SECONDARY4 = 0x6,
	LOADOUT_SLOT_SECONDARY5 = 0x7,
	LOADOUT_SLOT_SMG0 = 0x8,
	LOADOUT_SLOT_SMG1 = 0x9,
	LOADOUT_SLOT_SMG2 = 0xa,
	LOADOUT_SLOT_SMG3 = 0xb,
	LOADOUT_SLOT_SMG4 = 0xc,
	LOADOUT_SLOT_SMG5 = 0xd,
	LOADOUT_SLOT_RIFLE0 = 0xe,
	LOADOUT_SLOT_RIFLE1 = 0xf,
	LOADOUT_SLOT_RIFLE2 = 0x10,
	LOADOUT_SLOT_RIFLE3 = 0x11,
	LOADOUT_SLOT_RIFLE4 = 0x12,
	LOADOUT_SLOT_RIFLE5 = 0x13,
	LOADOUT_SLOT_HEAVY0 = 0x14,
	LOADOUT_SLOT_HEAVY1 = 0x15,
	LOADOUT_SLOT_HEAVY2 = 0x16,
	LOADOUT_SLOT_HEAVY3 = 0x17,
	LOADOUT_SLOT_HEAVY4 = 0x18,
	LOADOUT_SLOT_HEAVY5 = 0x19,
	LOADOUT_SLOT_FIRST_WHEEL_WEAPON = 0x2,
	LOADOUT_SLOT_LAST_WHEEL_WEAPON = 0x19,
	LOADOUT_SLOT_FIRST_PRIMARY_WEAPON = 0x8,
	LOADOUT_SLOT_LAST_PRIMARY_WEAPON = 0x19,
	LOADOUT_SLOT_FIRST_WHEEL_GRENADE = 0x1a,
	LOADOUT_SLOT_GRENADE0 = 0x1a,
	LOADOUT_SLOT_GRENADE1 = 0x1b,
	LOADOUT_SLOT_GRENADE2 = 0x1c,
	LOADOUT_SLOT_GRENADE3 = 0x1d,
	LOADOUT_SLOT_GRENADE4 = 0x1e,
	LOADOUT_SLOT_GRENADE5 = 0x1f,
	LOADOUT_SLOT_LAST_WHEEL_GRENADE = 0x1f,
	LOADOUT_SLOT_EQUIPMENT0 = 0x20,
	LOADOUT_SLOT_EQUIPMENT1 = 0x21,
	LOADOUT_SLOT_EQUIPMENT2 = 0x22,
	LOADOUT_SLOT_EQUIPMENT3 = 0x23,
	LOADOUT_SLOT_EQUIPMENT4 = 0x24,
	LOADOUT_SLOT_EQUIPMENT5 = 0x25,
	LOADOUT_SLOT_FIRST_WHEEL_EQUIPMENT = 0x20,
	LOADOUT_SLOT_LAST_WHEEL_EQUIPMENT = 0x25,
	LOADOUT_SLOT_CLOTHING_CUSTOMPLAYER = 0x26,
	LOADOUT_SLOT_PET = 0x27,
	LOADOUT_SLOT_CLOTHING_FACEMASK = 0x28,
	LOADOUT_SLOT_CLOTHING_HANDS = 0x29,
	LOADOUT_SLOT_FIRST_COSMETIC = 0x29,
	LOADOUT_SLOT_LAST_COSMETIC = 0x29,
	LOADOUT_SLOT_CLOTHING_EYEWEAR = 0x2a,
	LOADOUT_SLOT_CLOTHING_HAT = 0x2b,
	LOADOUT_SLOT_CLOTHING_LOWERBODY = 0x2c,
	LOADOUT_SLOT_CLOTHING_TORSO = 0x2d,
	LOADOUT_SLOT_CLOTHING_APPEARANCE = 0x2e,
	LOADOUT_SLOT_MISC0 = 0x2f,
	LOADOUT_SLOT_MISC1 = 0x30,
	LOADOUT_SLOT_MISC2 = 0x31,
	LOADOUT_SLOT_MISC3 = 0x32,
	LOADOUT_SLOT_MISC4 = 0x33,
	LOADOUT_SLOT_MISC5 = 0x34,
	LOADOUT_SLOT_MISC6 = 0x35,
	LOADOUT_SLOT_MUSICKIT = 0x36,
	LOADOUT_SLOT_FLAIR0 = 0x37,
	LOADOUT_SLOT_SPRAY0 = 0x38,
	LOADOUT_SLOT_FIRST_ALL_CHARACTER = 0x36,
	LOADOUT_SLOT_LAST_ALL_CHARACTER = 0x38,
	LOADOUT_SLOT_COUNT = 0x39,
};

class CBasePlayerWeaponVData
{
public:
	DECLARE_SCHEMA_CLASS(CBasePlayerWeaponVData);
	SCHEMA_FIELD(bool, m_bBuiltRightHanded); // 0x108	
	SCHEMA_FIELD(bool, m_bAllowFlipping); // 0x109	
	SCHEMA_FIELD(bool, m_bIsFullAuto); // 0x10a	
	SCHEMA_FIELD(int32_t, m_nNumBullets); // 0x10c	
	SCHEMA_FIELD(CUtlString ,m_sMuzzleAttachment); // 0x110	
	SCHEMA_FIELD(int32_t, m_iMaxClip1); // 0x1fc	
	SCHEMA_FIELD(int32_t, m_iMaxClip2); // 0x200	
	SCHEMA_FIELD(int32_t, m_iDefaultClip1); // 0x204	
	SCHEMA_FIELD(int32_t, m_iDefaultClip2); // 0x208	
	SCHEMA_FIELD(int32_t, m_iWeight); // 0x20c	
	SCHEMA_FIELD(bool, m_bAutoSwitchTo); // 0x210	
	SCHEMA_FIELD(bool, m_bAutoSwitchFrom); // 0x211	
	SCHEMA_FIELD(int32_t, m_iSlot); // 0x238	
	SCHEMA_FIELD(int32_t, m_iPosition); // 0x23c	
	
};

class CCSWeaponBaseVData : public CBasePlayerWeaponVData
{
public:
	DECLARE_SCHEMA_CLASS(CCSWeaponBaseVData);
	SCHEMA_FIELD(CSWeaponType, m_WeaponType);
	SCHEMA_FIELD(CSWeaponCategory, m_WeaponCategory);
	SCHEMA_FIELD(int32_t, m_GearSlotPosition);
	SCHEMA_FIELD(int32_t, m_nPrice);
	SCHEMA_FIELD(int32_t, m_nKillAward);
	SCHEMA_FIELD(int32_t, m_nPrimaryReserveAmmoMax);
	SCHEMA_FIELD(int32_t, m_nSecondaryReserveAmmoMax);	
	SCHEMA_FIELD(bool, m_bMeleeWeapon); // 0xc10	
	SCHEMA_FIELD(bool, m_bHasBurstMode); // 0xc11	
	SCHEMA_FIELD(bool, m_bIsRevolver); // 0xc12	
	SCHEMA_FIELD(bool, m_bCannotShootUnderwater); // 0xc13	
	SCHEMA_FIELD(CUtlString, m_szName); // 0xc18	
	// MPropertyFriendlyName "Player Animation Extension"
	SCHEMA_FIELD(CUtlString, m_szAnimExtension); // 0xc20	
	SCHEMA_FIELD(int32_t, m_nCrosshairMinDistance); // 0xc2c	
	SCHEMA_FIELD(int32_t, m_nCrosshairDeltaDistance); // 0xc30	
	SCHEMA_FIELD(float, m_flInaccuracyJumpInitial); // 0xcac	
	SCHEMA_FIELD(float, m_flInaccuracyJumpApex); // 0xcb0	
	SCHEMA_FIELD(float, m_flInaccuracyReload); // 0xcb4	
	// MPropertyStartGroup "Firing"
	SCHEMA_FIELD(int32_t, m_nRecoilSeed); // 0xcb8	
	SCHEMA_FIELD(int32_t, m_nSpreadSeed); // 0xcbc	
	SCHEMA_FIELD(float, m_flTimeToIdleAfterFire); // 0xcc0	
	SCHEMA_FIELD(float, m_flIdleInterval); // 0xcc4	
	SCHEMA_FIELD(float, m_flAttackMovespeedFactor); // 0xcc8	
	SCHEMA_FIELD(float, m_flHeatPerShot); // 0xccc	
	SCHEMA_FIELD(float, m_flInaccuracyPitchShift); // 0xcd0	
	SCHEMA_FIELD(float, m_flInaccuracyAltSoundThreshold); // 0xcd4	
	SCHEMA_FIELD(float, m_flBotAudibleRange); // 0xcd8	
	// MPropertyStartGroup "Zooming"
	SCHEMA_FIELD(bool, m_bUnzoomsAfterShot); // 0xce8	
	SCHEMA_FIELD(bool, m_bHideViewModelWhenZoomed); // 0xce9	
	SCHEMA_FIELD(int32_t, m_nZoomLevels); // 0xcec	
	SCHEMA_FIELD(int32_t, m_nZoomFOV1); // 0xcf0	
	SCHEMA_FIELD(int32_t, m_nZoomFOV2); // 0xcf4	
	SCHEMA_FIELD(float, m_flZoomTime0); // 0xcf8	
	SCHEMA_FIELD(float, m_flZoomTime1); // 0xcfc	
	SCHEMA_FIELD(float, m_flZoomTime2); // 0xd00	
	// MPropertyStartGroup "Iron Sights"
	SCHEMA_FIELD(float, m_flIronSightPullUpSpeed); // 0xd04	
	SCHEMA_FIELD(float, m_flIronSightPutDownSpeed); // 0xd08	
	SCHEMA_FIELD(float, m_flIronSightFOV); // 0xd0c	
	SCHEMA_FIELD(float, m_flIronSightPivotForward); // 0xd10	
	SCHEMA_FIELD(float, m_flIronSightLooseness); // 0xd14	
	//SCHEMA_FIELD(QAngle, m_angPivotAngle); // 0xd18	
	//SCHEMA_FIELD(Vector, m_vecIronSightEyePos); // 0xd24	
	// MPropertyStartGroup "Damage"
	SCHEMA_FIELD(int32_t, m_nDamage); // 0xd30	
	SCHEMA_FIELD(float, m_flHeadshotMultiplier); // 0xd34	
	SCHEMA_FIELD(float, m_flArmorRatio); // 0xd38	
	SCHEMA_FIELD(float, m_flPenetration); // 0xd3c	
	SCHEMA_FIELD(float, m_flRange); // 0xd40	
	SCHEMA_FIELD(float, m_flRangeModifier); // 0xd44	
	SCHEMA_FIELD(float, m_flFlinchVelocityModifierLarge); // 0xd48	
	SCHEMA_FIELD(float, m_flFlinchVelocityModifierSmall); // 0xd4c	
	// MPropertyStartGroup "Recovery"
	SCHEMA_FIELD(float, m_flRecoveryTimeCrouch); // 0xd50	
	SCHEMA_FIELD(float, m_flRecoveryTimeStand); // 0xd54	
	SCHEMA_FIELD(float, m_flRecoveryTimeCrouchFinal); // 0xd58	
	SCHEMA_FIELD(float, m_flRecoveryTimeStandFinal); // 0xd5c	
	SCHEMA_FIELD(int32_t, m_nRecoveryTransitionStartBullet); // 0xd60	
	SCHEMA_FIELD(int32_t, m_nRecoveryTransitionEndBullet); // 0xd64	
	// MPropertyStartGroup "Grenade Data"
	SCHEMA_FIELD(float, m_flThrowVelocity); // 0xd68	
	SCHEMA_FIELD(Vector, m_vSmokeColor); // 0xd6c	
	SCHEMA_FIELD(CUtlString, m_szAnimClass); // 0xd78	
};
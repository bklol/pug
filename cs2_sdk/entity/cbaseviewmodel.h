#pragma once
#include "cbaseplayerweapon.h"
#include "cbaseentity.h"
#include "ehandle.h"

class CBaseViewModel
{
	DECLARE_SCHEMA_CLASS(CBaseViewModel);
	SCHEMA_FIELD(Vector, m_vecLastFacing) // 0x898	
	SCHEMA_FIELD(uint32_t, m_nViewModelIndex) // 0x8a4	
	SCHEMA_FIELD(uint32_t, m_nAnimationParity) // 0x8a8	
	SCHEMA_FIELD(float, m_flAnimationStartTime) // 0x8ac	
	SCHEMA_FIELD(CHandle< CBasePlayerWeapon >, m_hWeapon) // 0x8b0	
	SCHEMA_FIELD(CUtlSymbolLarge, m_sVMName) // 0x8b8	
	SCHEMA_FIELD(CUtlSymbolLarge, m_sAnimationPrefix) // 0x8c0	
	SCHEMA_FIELD(int32_t, m_oldLayer) // 0x8cc	
	SCHEMA_FIELD(float, m_oldLayerStartTime) // 0x8d0	
};
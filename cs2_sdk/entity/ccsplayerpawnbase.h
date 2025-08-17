#pragma once
#include "cbaseplayerpawn.h"
class CCSPlayerController;
class CCSPlayerPawnBase: public CBasePlayerPawn
{
public:
	DECLARE_SCHEMA_CLASS_INLINE(CCSPlayerPawnBase)
	SCHEMA_FIELD(CCSPlayer_ViewModelServices*, m_pViewModelServices)
	SCHEMA_FIELD(CHandle< CCSPlayerController >, m_hOriginalController)
};
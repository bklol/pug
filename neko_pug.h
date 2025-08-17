#pragma once

#include <ISmmPlugin.h>
#include <sh_vector.h>
#include <iserver.h>
#include <functional>
#include "deque"
#include "common.h"
#include "addresses.h"
#include "icvar.h"
#include "interface.h"
#include "tier0/dbg.h"
#include "tier0/memdbgon.h"
#include "plat.h"
#include "igameevents.h"
#include "schemasystem/schemasystem.h"
#include "entitysystem.h"
#include "appframework/IAppSystem.h"
#include "entity/ccsplayercontroller.h"
#include "PlayerMap.h"
#include "GameMap.h"

extern Player g_Player[MAXPLAYERS + 1];
extern CGlobalVars *gpGlobals;

class neko_pug : public ISmmPlugin, public IMetamodListener
{
public:
	bool Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late);
	bool Unload(char *error, size_t maxlen);
	void Hook_StartupServer(const GameSessionConfiguration_t& config, ISource2WorldSession*, const char*);
	bool Pause(char *error, size_t maxlen);
	bool Unpause(char *error, size_t maxlen);
	void Hook_DispatchConCommand(ConCommandRef cmd, const CCommandContext& ctx, const CCommand& args);
	void ClientDisconnect( CPlayerSlot slot, ENetworkDisconnectionReason reason, const char *pszName, uint64 xuid, const char *pszNetworkID  );
	void Hook_ClientPutInServer( CPlayerSlot slot, char const *pszName, int type, uint64 xuid );
	int Hook_LoadEventsFromFile(const char *filename, bool bSearchAll);
	void AllPluginsLoaded();
public:
	void NextFrame(std::function<void()> fn);
	const char *GetAuthor();
	const char *GetName();
	const char *GetDescription();
	const char *GetURL();
	const char *GetLicense();
	const char *GetVersion();
	const char *GetDate();
	const char *GetLogTag();
private: 
	void GameFrame(bool simulating, bool bFirstTick, bool bLastTick);
	std::deque<std::function<void()>> m_nextFrame;
};

class CPlayerDeathEvent : public IGameEventListener2
{
	void FireGameEvent(IGameEvent* event) override;
};

class CPlayerSpawnEvent : public IGameEventListener2
{
	void FireGameEvent(IGameEvent* event) override;
};

extern neko_pug g_neko_pug;

PLUGIN_GLOBALVARS();

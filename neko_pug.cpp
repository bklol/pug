#include "neko_pug.h"
#include "pugsystem.h"

CGameEntitySystem *g_pEntitySystem = nullptr;
CGlobalVars *gpGlobals = nullptr;
IVEngineServer2 *g_pEngineServer2;
CSchemaSystem *g_pSchemaSystem2 = nullptr;
IGameEventManager2 *g_gameEventManager = nullptr;

MatchStateManager matchStateManager;
Player g_Player[MAXPLAYERS + 1];
int g_iLoadEventsFromFileId = -1;
CPlayerDeathEvent g_PlayerDeathEvent;

void Message(const char *msg, ...)
{
    va_list args;
    va_start(args, msg);

    char buf[1024] = {};
    V_vsnprintf(buf, sizeof(buf) - 1, msg, args);

    ConColorMsg(Color(255, 0, 255, 255), "[NEKO PUG] %s", buf);

    va_end(args);
}

void Panic(const char *msg, ...)
{
    va_list args;
    va_start(args, msg);

    char buf[1024] = {};
    V_vsnprintf(buf, sizeof(buf) - 1, msg, args);

    if (CommandLine()->HasParm("-dedicated"))
        Warning("[NEKO PUG] %s", buf);
#ifdef _WIN32
    else
        MessageBoxA(nullptr, buf, "Warning", 0);
#endif

    va_end(args);
}

class GameSessionConfiguration_t { };

SH_DECL_HOOK3_void(INetworkServerService, StartupServer, SH_NOATTRIB, 0, const GameSessionConfiguration_t&, ISource2WorldSession*, const char*);
SH_DECL_HOOK3_void(IServerGameDLL, GameFrame, SH_NOATTRIB, 0, bool, bool, bool);
SH_DECL_HOOK2(IGameEventManager2, LoadEventsFromFile, SH_NOATTRIB, 0, int, const char *, bool);
SH_DECL_HOOK3_void(ICvar, DispatchConCommand, SH_NOATTRIB, 0, ConCommandRef, const CCommandContext&, const CCommand&);
SH_DECL_HOOK5_void(IServerGameClients, ClientDisconnect, SH_NOATTRIB, 0, CPlayerSlot, ENetworkDisconnectionReason, const char *, uint64, const char *);
SH_DECL_HOOK4_void(IServerGameClients, ClientPutInServer, SH_NOATTRIB, 0, CPlayerSlot, char const *, int, uint64);

CGlobalVars *GetGameGlobals()
{
    INetworkGameServer *server = g_pNetworkServerService->GetIGameServer();

    if (!server)
        return nullptr;

    return server->GetGlobals();
}

CGameEntitySystem *GameEntitySystem()
{
    return *reinterpret_cast<CGameEntitySystem **>((uintptr_t)(g_pGameResourceServiceServer) + WIN_LINUX(88, 80));
}

neko_pug g_neko_pug;
PLUGIN_EXPOSE(neko_pug, g_neko_pug);

bool neko_pug::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
    PLUGIN_SAVEVARS();

    GET_V_IFACE_CURRENT(GetEngineFactory, g_pEngineServer2, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
    GET_V_IFACE_CURRENT(GetEngineFactory, g_pGameResourceServiceServer, IGameResourceService, GAMERESOURCESERVICESERVER_INTERFACE_VERSION);
    GET_V_IFACE_CURRENT(GetEngineFactory, g_pSchemaSystem, ISchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);    
    GET_V_IFACE_CURRENT(GetEngineFactory, g_pCVar, ICvar, CVAR_INTERFACE_VERSION);
    GET_V_IFACE_CURRENT(GetServerFactory, g_pSource2GameClients, IServerGameClients, INTERFACEVERSION_SERVERGAMECLIENTS);
    
    GET_V_IFACE_ANY(GetServerFactory, g_pSource2Server, ISource2Server, SOURCE2SERVER_INTERFACE_VERSION);
    GET_V_IFACE_ANY(GetEngineFactory, g_pNetworkServerService, INetworkServerService, NETWORKSERVERSERVICE_INTERFACE_VERSION);

    SH_ADD_HOOK(IServerGameDLL, GameFrame, g_pSource2Server, SH_MEMBER(this, &neko_pug::GameFrame), true);
    SH_ADD_HOOK_MEMFUNC(ICvar, DispatchConCommand, g_pCVar, this, &neko_pug::Hook_DispatchConCommand, false);    
    SH_ADD_HOOK_MEMFUNC(INetworkServerService, StartupServer, g_pNetworkServerService, this, &neko_pug::Hook_StartupServer, true);
    SH_ADD_HOOK(IServerGameClients, ClientDisconnect, g_pSource2GameClients, SH_MEMBER(this, &neko_pug::ClientDisconnect), true);
    SH_ADD_HOOK(IServerGameClients, ClientPutInServer, g_pSource2GameClients, SH_MEMBER(this, &neko_pug::Hook_ClientPutInServer), true);
    
    g_pEntitySystem = GameEntitySystem();
    
    addresses::Initialize();
    
    auto pCGameEventManagerVTable = (IGameEventManager2*)modules::server->FindVirtualTable("CGameEventManager");
    g_iLoadEventsFromFileId = SH_ADD_DVPHOOK(IGameEventManager2, LoadEventsFromFile, pCGameEventManagerVTable, SH_MEMBER(this, &neko_pug::Hook_LoadEventsFromFile), false);
    
    if(late)
    {
        gpGlobals = GetGameGlobals();    
        g_pEntitySystem = GameEntitySystem();
        g_gameEventManager->AddListener(&g_PlayerDeathEvent, "player_death", true);
        g_PugSystem.Init();
    }
    
    return true;
}

bool neko_pug::Unload(char *error, size_t maxlen)
{
    SH_REMOVE_HOOK_MEMFUNC(ICvar, DispatchConCommand, g_pCVar, this, &neko_pug::Hook_DispatchConCommand, false);
    SH_REMOVE_HOOK_MEMFUNC(INetworkServerService, StartupServer, g_pNetworkServerService, this, &neko_pug::Hook_StartupServer, true);
    SH_REMOVE_HOOK(IServerGameDLL, GameFrame, g_pSource2Server, SH_MEMBER(this, &neko_pug::GameFrame), true);
    SH_REMOVE_HOOK(IServerGameClients, ClientDisconnect, g_pSource2GameClients, SH_MEMBER(this, &neko_pug::ClientDisconnect), true);
    SH_REMOVE_HOOK(IServerGameClients, ClientPutInServer, g_pSource2GameClients, SH_MEMBER(this, &neko_pug::Hook_ClientPutInServer), true);
    SH_REMOVE_HOOK_ID(g_iLoadEventsFromFileId);
    
    g_gameEventManager->RemoveListener(&g_PlayerDeathEvent);
    return true;
}

void neko_pug::Hook_StartupServer(const GameSessionConfiguration_t& config, ISource2WorldSession*, const char*)
{
    Message("Server starting up, initializing PUG system\n");
    gpGlobals = GetGameGlobals();
    g_pEntitySystem = GameEntitySystem();
    g_gameEventManager->AddListener(&g_PlayerDeathEvent, "player_death", true);
    g_PugSystem.Init();
}

void neko_pug::GameFrame(bool simulating, bool bFirstTick, bool bLastTick)
{
    while (!m_nextFrame.empty()) {
        m_nextFrame.front()();
        m_nextFrame.pop_front();
    }
    
    g_PugSystem.OnFrame();
}

void neko_pug::Hook_ClientPutInServer(CPlayerSlot slot, char const *pszName, int type, uint64 xuid)
{
    g_PugSystem.OnPlayerJoin(slot.Get(), xuid);
}

void neko_pug::ClientDisconnect(CPlayerSlot slot, ENetworkDisconnectionReason reason, const char *pszName, uint64 xuid, const char *pszNetworkID)
{
    if (!xuid)
        return;
        
    g_PugSystem.OnPlayerDisconnect(slot.Get());
}

void neko_pug::Hook_DispatchConCommand(ConCommandRef cmdHandle, const CCommandContext& ctx, const CCommand& args)
{
    if (!g_pEntitySystem)
        return;

    CCSPlayerController* pController = (CCSPlayerController*)g_pEntitySystem->GetEntityInstance((CEntityIndex)(ctx.GetPlayerSlot().Get() + 1));
    if (!pController || !pController->IsController())
        return;

    if (args.ArgC() > 0 && (*args[0] == '!' || *args[0] == '.')) {
        const char* command = args[0] + 1;
        g_PugSystem.OnPlayerCommand(pController, command);
    }
}

int neko_pug::Hook_LoadEventsFromFile(const char *filename, bool bSearchAll)
{
    ExecuteOnce(g_gameEventManager = META_IFACEPTR(IGameEventManager2));
    
    RETURN_META_VALUE(MRES_IGNORED, 0);
}

void neko_pug::NextFrame(std::function<void()> fn)
{
    m_nextFrame.push_back(fn);
}

void CPlayerDeathEvent::FireGameEvent(IGameEvent* event)
{
    if (g_PugSystem.IsMatchLive()) 
	{

    }
}

void neko_pug::AllPluginsLoaded()
{

}

bool neko_pug::Pause(char *error, size_t maxlen)
{
    return true;
}

bool neko_pug::Unpause(char *error, size_t maxlen)
{
    return true;
}

const char *neko_pug::GetLicense()
{
    return "";
}

const char *neko_pug::GetVersion()
{
    return "0.0.1";
}

const char *neko_pug::GetDate()
{
    return __DATE__;
}

const char *neko_pug::GetLogTag()
{
    return "NEKO_PUG";
}

const char *neko_pug::GetAuthor()
{
    return "most copied from cs2fix, bklol";
}

const char *neko_pug::GetDescription()
{
    return "NEKO PUG System for CS2";
}

const char *neko_pug::GetName()
{
    return "NEKO PUG";
}

const char *neko_pug::GetURL()
{
    return "https://github.com/bklol";
}
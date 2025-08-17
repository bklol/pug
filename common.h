#pragma once

#include "platform.h"
#include "irecipientfilter.h"
#include "entitysystem.h"

extern CGameEntitySystem *g_pEntitySystem;

// #define HOOK_CONVARS
// #define HOOK_CONCOMMANDS
// #define USE_TICKRATE
// #define USE_DEBUG_CONSOLE

#define CS_TEAM_NONE        0
#define CS_TEAM_SPECTATOR   1
#define CS_TEAM_T           2
#define CS_TEAM_CT          3

#define HUD_PRINTNOTIFY		1
#define HUD_PRINTCONSOLE	2
#define HUD_PRINTTALK		3
#define HUD_PRINTCENTER		4

#ifdef _WIN32
#define ROOTBIN "/bin/win64/"
#define GAMEBIN "/csgo/bin/win64/"
#else
#define ROOTBIN "/bin/linuxsteamrt64/"
#define GAMEBIN "/csgo/bin/linuxsteamrt64/"
#endif

#ifdef _WIN32
#define WIN_LINUX(win, linux) win
#else
#define WIN_LINUX(win, linux) linux
#endif

class CCSPlayerController;
void Message(const char *, ...);
void Panic(const char *, ...);

void ClientPrintAll(int destination, const char *msg, ...);
void ClientPrint(CCSPlayerController *player, int destination, const char *msg, ...);


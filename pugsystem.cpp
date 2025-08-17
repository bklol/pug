#include "pugsystem.h"
#include "addresses.h"
#include "tier0/dbg.h"
#include <cstdio>
#include <algorithm>

extern CGlobalVars* gpGlobals;
extern Player g_Player[MAXPLAYERS + 1];
extern IVEngineServer2* g_pEngineServer2;

CPugSystem g_PugSystem;

void CPugSystem::Init() 
{
    m_matchState.Set(MatchStateManager::State::WarmUp);
    m_pMenus = nullptr;
    m_bVoteInProgress = false;
    
    std::random_device rd;
    m_rng = std::mt19937(rd());
	
	m_bMatchPaused = false;
    m_flPauseStartTime = 0.0f;
    m_iPausingTeam = -1;
    m_teamPauses[TeamSide::TeamA] = { 2, 0.0f };
    m_teamPauses[TeamSide::TeamB] = { 2, 0.0f };
	
    m_mapPool = {
        "de_dust2", "de_inferno", "de_mirage", 
        "de_nuke", "de_overpass", "de_vertigo", 
        "de_ancient", "de_anubis"
    };
    
    m_pMenus = reinterpret_cast<IMenusApi*>(g_SMAPI->MetaFactory(Menus_INTERFACE, nullptr, nullptr));
    if (!m_pMenus) {
        Panic("Failed to get menus interface\n");
    }
}

void CPugSystem::RequestPause(CCSPlayerController* pController) {
    if (!pController || !pController->IsController()) return;
    
    if (!IsMatchLive()) {
        pController->ClientPrint(HUD_PRINTTALK, "[\x03NEKO PUG\x01] 比赛未开始，无法暂停！");
        return;
    }
    
    if (m_bMatchPaused) {
        pController->ClientPrint(HUD_PRINTTALK, "[\x03NEKO PUG\x01] 比赛已暂停！");
        return;
    }
    
    int slot = pController->GetPlayerSlot();
    TeamSide team = g_Player[slot].team;
    
    if (team == TeamSide::None) {
        pController->ClientPrint(HUD_PRINTTALK, "[\x03NEKO PUG\x01] 只有比赛中的玩家可以暂停！");
        return;
    }
    
    auto& pauseInfo = m_teamPauses[team];
    
    if (pauseInfo.remainingPauses <= 0) {
        pController->ClientPrint(HUD_PRINTTALK, "[\x03NEKO PUG\x01] 你的队伍已用完所有暂停次数！");
        return;
    }

    m_pauseVotes[slot] = true;

    int teamPlayers = 0;
    int votes = 0;
    
    for (int i = 0; i < gpGlobals->maxClients; ++i) {
        if (g_Player[i].team == team) {
            teamPlayers++;
            if (m_pauseVotes.count(i) && m_pauseVotes[i]) {
                votes++;
            }
        }
    }
    
    if (votes >= (teamPlayers + 1) / 2) 
	{
        PauseMatch(team);
    } 
	else 
	{
        char buffer[128];
        snprintf(buffer, sizeof(buffer), 
                "[\x03NEKO PUG\x01] 暂停投票中: %d/%d 同意 (需要 %d)",
                votes, teamPlayers, (teamPlayers + 1) / 2);
        TeamMessage(static_cast<int>(team), buffer);
        
        snprintf(buffer, sizeof(buffer), 
                "[\x03NEKO PUG\x01] 使用 .pause 同意暂停");
        TeamMessage(static_cast<int>(team), buffer);
    }
}

void CPugSystem::PauseMatch(TeamSide team) {
    if (m_bMatchPaused) return;
    
    m_bMatchPaused = true;
    m_flPauseStartTime = gpGlobals->curtime;
    m_iPausingTeam = static_cast<int>(team);
    m_teamPauses[team].remainingPauses--;
    m_teamPauses[team].lastPauseTime = gpGlobals->curtime;
    m_pauseVotes.clear();
    
	/*
    ConVar* mp_freezetime = g_pCVar->FindVar("mp_freezetime");
    if (mp_freezetime) {
        mp_freezetime->SetValue(30);
    }
    */
	
    char buffer[128];
    snprintf(buffer, sizeof(buffer), 
            "[\x03NEKO PUG\x01] %s 队使用了暂停 (剩余暂停次数: %d)",
            team == TeamSide::TeamA ? "A" : "B", 
            m_teamPauses[team].remainingPauses);
    BroadcastMessage(buffer);
    
    snprintf(buffer, sizeof(buffer), 
            "[\x03NEKO PUG\x01] 暂停将持续30秒，或使用 .unpause 提前取消");
    BroadcastMessage(buffer);
}

void CPugSystem::RequestUnpause(CCSPlayerController* pController) {
    if (!pController || !pController->IsController()) return;
    
    if (!m_bMatchPaused) {
        pController->ClientPrint(HUD_PRINTTALK, "[\x03NEKO PUG\x01] 比赛未暂停！");
        return;
    }
    
    int slot = pController->GetPlayerSlot();
    
    m_pauseVotes[slot] = true;
	
    int totalPlayers = 0;
    int votes = 0;
    
    for (int i = 0; i < gpGlobals->maxClients; ++i) {
        CCSPlayerController* pPlayer = CCSPlayerController::FromSlot(i);
        if (pPlayer && pPlayer->IsController() && pPlayer->Get_Team() > 1) {
            totalPlayers++;
            if (m_pauseVotes.count(i) && m_pauseVotes[i]) {
                votes++;
            }
        }
    }
    
    if (votes == totalPlayers) 
	{
        UnpauseMatch();
    } 
	else 
	{
        char buffer[128];
        snprintf(buffer, sizeof(buffer), 
                "[\x03NEKO PUG\x01] 取消暂停投票中: %d/%d 同意 (需要所有玩家同意)",
                votes, totalPlayers);
        BroadcastMessage(buffer);
        
        snprintf(buffer, sizeof(buffer), 
                "[\x03NEKO PUG\x01] 使用 .unpause 同意取消暂停");
        BroadcastMessage(buffer);
    }
}

void CPugSystem::UnpauseMatch() {
    if (!m_bMatchPaused) return;
    
    m_bMatchPaused = false;
    m_pauseVotes.clear();
    
	/*
    ConVar* mp_freezetime = g_pCVar->FindVar("mp_freezetime");
    if (mp_freezetime) {
        mp_freezetime->SetValue(0);
    }
    */
	
    BroadcastMessage("[\x03NEKO PUG\x01] 比赛已恢复！");
}

bool CPugSystem::IsMatchPaused() const 
{
    return m_bMatchPaused;
}

void CPugSystem::OnFrame() 
{
    if (!m_nextFrameActions.empty()) {
        for (auto& action : m_nextFrameActions) 
		{
            action();
        }
        m_nextFrameActions.clear();
    }
	
    if (m_bMatchPaused && gpGlobals->curtime - m_flPauseStartTime > 30.0f) {
        UnpauseMatch();
        BroadcastMessage("[\x03NEKO PUG\x01] 暂停时间已到，比赛自动恢复！");
    }
	
    switch (m_matchState.Get()) 
	{
        case MatchStateManager::State::WarmUp:
            for (int i = 0; i < gpGlobals->maxClients; ++i) 
			{
                CCSPlayerController* pController = CCSPlayerController::FromSlot(i);
                if (pController && pController->IsController() && pController->Get_Team() > 1) 
				{
                    ReadyHintMsg(pController);
                }
            }
            break;
            
        case MatchStateManager::State::WaitingForGameVote:
            if (!m_bVoteInProgress) 
			{
                StartMapVote();
            }
            break;
            
        case MatchStateManager::State::PickPlayers:
            if (m_captains.empty()) 
			{
                SelectCaptains();
            }
            break;
            
        case MatchStateManager::State::Live:
            break;
            
        default:
            break;
    }
}

void CPugSystem::StartMatch() 
{
    if (m_matchState.Get() != MatchStateManager::State::WarmUp) 
		return;

    if (GetReadyPlayerCount() >= 10) 
	{
        m_matchState.Set(MatchStateManager::State::WaitingForGameVote);
        BroadcastMessage("比赛即将开始，请投票选择地图");
    } 
	else 
	{
        BroadcastMessage("准备玩家不足10人，无法开始比赛");
    }
}

void CPugSystem::EndMatch() {
    m_matchState.Set(MatchStateManager::State::MatchEnd);
    BroadcastMessage("比赛已结束");
    
    for (int i = 0; i < gpGlobals->maxClients; i++) {
        g_Player[i].Init();
    }
}

void CPugSystem::RestartMatch() {
    EndMatch();
    m_matchState.Set(MatchStateManager::State::WarmUp);
    BroadcastMessage("比赛已重置，请重新准备");
}

void CPugSystem::OnPlayerCommand(CCSPlayerController* pController, const char* command) {
    if (!pController || !command) return;

    int slot = pController->GetPlayerSlot();
    
    if (!V_stricmp("r", command) || !V_stricmp("ready", command)) 
	{
        SetPlayerReady(slot, !g_Player[slot].isReady);
        pController->ClientPrint(HUD_PRINTTALK, g_Player[slot].isReady ? 
            "[\x03NEKO PUG\x01] 你已准备！" : "[\x03NEKO PUG\x01] 你已取消准备！");
        CheckReady();
    }
    else if (!V_stricmp("score", command)) 
	{
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "准备状态: %d/10", GetReadyPlayerCount());
        pController->ClientPrint(HUD_PRINTTALK, buffer);
    }
    else if (!V_stricmp("leave", command) && !IsMatchLive()) 
	{
        pController->SwitchTeam(1);
        g_Player[slot].Init();
    }
    if (!V_stricmp("p", command) || !V_stricmp("pause", command)) {
        RequestPause(pController);
    }
    else if (!V_stricmp("up", command) || !V_stricmp("unpause", command)) {
        RequestUnpause(pController);
    }
    else if (pController->IsAdmin()) 
	{
        if (!V_stricmp("start", command)) {
            StartMatch();
        }
        else if (!V_stricmp("end", command)) {
            EndMatch();
        }
        else if (!V_stricmp("restart", command)) {
            RestartMatch();
        }
        else if (!V_stricmp("swap", command)) {
            SwapTeams();
        }
    }
}

void CPugSystem::SetPlayerReady(int slot, bool ready) {
    if (slot < 0 || slot >= MAXPLAYERS) return;
    
    g_Player[slot].isReady = ready;
    g_Player[slot].readyTime = gpGlobals->curtime;
    
    if (ready && g_Player[slot].team == TeamSide::None) {
        AssignTeams();
    }
}

void CPugSystem::CheckReady() {
    if (m_matchState.Get() != MatchStateManager::State::WarmUp) return;
    
    int readyCount = GetReadyPlayerCount();
    
    if (readyCount >= 10) {
        m_nextFrameActions.push_back([this]() {
            StartMatch();
        });
    }
}

void CPugSystem::AssignTeams() {
    int teamA = 0, teamB = 0;
    
    for (int i = 0; i < gpGlobals->maxClients; i++) {
        CCSPlayerController* pController = CCSPlayerController::FromSlot(i);
        if (pController && pController->IsController() && g_Player[i].isReady) {
            if (teamA <= teamB) {
                pController->SwitchTeam(2);
                g_Player[i].team = TeamSide::TeamA;
                teamA++;
            } else {
                pController->SwitchTeam(3);
                g_Player[i].team = TeamSide::TeamB;
                teamB++;
            }
        }
    }
}

void CPugSystem::SwapTeams() {
    if (!IsMatchLive()) return;
    
    for (int i = 0; i < gpGlobals->maxClients; i++) {
        CCSPlayerController* pController = CCSPlayerController::FromSlot(i);
        if (pController && pController->IsController()) {
            if (pController->Get_Team() == 2) {
                pController->SwitchTeam(3);
                g_Player[i].team = TeamSide::TeamB;
            } else if (pController->Get_Team() == 3) {
                pController->SwitchTeam(2);
                g_Player[i].team = TeamSide::TeamA;
            }
        }
    }
    
    BroadcastMessage("队伍已交换");
}

int CPugSystem::GetReadyPlayerCount(int team) const {
    int count = 0;
    for (int i = 0; i < gpGlobals->maxClients; i++) {
        if (g_Player[i].isReady && 
            (team == -1 || (team == 2 && g_Player[i].team == TeamSide::TeamA) || 
                           (team == 3 && g_Player[i].team == TeamSide::TeamB))) {
            count++;
        }
    }
    return count;
}

int CPugSystem::GetPlayerCount(int team) const {
    int count = 0;
    for (int i = 0; i < gpGlobals->maxClients; i++) {
        CCSPlayerController* pController = CCSPlayerController::FromSlot(i);
        if (pController && pController->IsController() && 
            (team == -1 || pController->Get_Team() == team)) {
            count++;
        }
    }
    return count;
}

void CPugSystem::ReadyHintMsg(CCSPlayerController* pController) const {
    char buffer[256];
    int readyCount = GetReadyPlayerCount();
    
    snprintf(buffer, sizeof(buffer),
             "输入 !r 准备\n已准备玩家: %d/10\n当前状态: %s",
             readyCount, m_matchState.ToString());
             
    addresses::ClientPrint(pController, HUD_PRINTCENTER, buffer, nullptr, nullptr, nullptr, nullptr);
}

void CPugSystem::BroadcastMessage(const char* message) const {
    for (int i = 0; i < gpGlobals->maxClients; ++i) {
        CCSPlayerController* pController = CCSPlayerController::FromSlot(i);
        if (pController && pController->IsController()) {
            pController->ClientPrint(HUD_PRINTTALK, message);
        }
    }
}

void CPugSystem::TeamMessage(int team, const char* message) const {
    for (int i = 0; i < gpGlobals->maxClients; ++i) {
        CCSPlayerController* pController = CCSPlayerController::FromSlot(i);
        if (pController && pController->IsController() && pController->Get_Team() == team) {
            pController->ClientPrint(HUD_PRINTTALK, message);
        }
    }
}

bool CPugSystem::IsMatchLive() const {
    return m_matchState.IsLive();
}

MatchStateManager::State CPugSystem::GetMatchState() const {
    return m_matchState.Get();
}

void CPugSystem::OnPlayerJoin(int slot, uint64 xuid) {
    if (slot < 0 || slot >= MAXPLAYERS) return;
    
    g_Player[slot].Init();
    g_Player[slot].playerId = xuid;
    
    if (IsMatchLive()) {
        CCSPlayerController* pController = CCSPlayerController::FromSlot(slot);
        if (pController) {
            pController->ClientPrint(HUD_PRINTTALK, "比赛正在进行中，你已加入观察者");
            pController->SwitchTeam(1);
        }
    }
}

void CPugSystem::OnPlayerDisconnect(int slot) {
    if (slot < 0 || slot >= MAXPLAYERS) return;
    
    g_Player[slot].Init();
    
    if (IsMatchLive() && GetPlayerCount(2) + GetPlayerCount(3) < 10) {
        BroadcastMessage("玩家数量不足，比赛暂停");
    }
}

void CPugSystem::ShowCaptainMenu(int slot) {
    if (!m_pMenus || slot < 0 || slot >= MAXPLAYERS) return;
    
    Menu menu;
    CreateCaptainMenu(menu);
    m_pMenus->DisplayPlayerMenu(menu, slot);
}

void CPugSystem::CreateCaptainMenu(Menu& menu) {
    m_pMenus->SetTitleMenu(menu, "队长菜单");
    m_pMenus->SetBackMenu(menu, false);
    
    for (int i = 0; i < gpGlobals->maxClients; ++i) {
        CCSPlayerController* pPlayer = CCSPlayerController::FromSlot(i);
        if (pPlayer && pPlayer->IsController() && !g_Player[i].isCaptain && g_Player[i].team == TeamSide::None) {
            char buffer[64];
            snprintf(buffer, sizeof(buffer), "选择玩家 %s", pPlayer->GetPlayerName());
            m_pMenus->AddItemMenu(menu, "select", buffer);
        }
    }
    
    m_pMenus->AddItemMenu(menu, "swap", "交换队伍");
    m_pMenus->AddItemMenu(menu, "random", "随机选人");
    m_pMenus->AddItemMenu(menu, "cancel", "取消");
    
    m_pMenus->SetCallback(menu, [this](const char* szItem, const char* szTranslate, int iItem, int iSlot) {
        this->OnCaptainMenuCallback(szItem, szTranslate, iItem, iSlot);
    });
}

void CPugSystem::OnCaptainMenuCallback(const char* szItem, const char* szTranslate, int iItem, int iSlot) {
    CCSPlayerController* pCaptain = CCSPlayerController::FromSlot(iSlot);
    if (!pCaptain || !pCaptain->IsController()) return;
    
    if (!strcmp(szItem, "swap")) {
        StartTeamVote();
    }
    else if (!strcmp(szItem, "random")) {
        AssignRandomTeams();
    }
    else if (!strcmp(szItem, "select")) {
        std::string playerName = szTranslate;
        size_t pos = playerName.find("选择玩家 ");
        if (pos != std::string::npos) {
            playerName = playerName.substr(pos + 7);
            SelectPlayer(iSlot, playerName.c_str());
        }
    }
    
    m_pMenus->ClosePlayerMenu(iSlot);
}

void CPugSystem::ShowVoteMenu(int slot, const std::string& title, const std::vector<std::string>& options) {
    if (!m_pMenus || slot < 0 || slot >= MAXPLAYERS) return;
    
    Menu menu;
    CreateVoteMenu(menu, title, options);
    m_pMenus->DisplayPlayerMenu(menu, slot);
}

void CPugSystem::CreateVoteMenu(Menu& menu, const std::string& title, const std::vector<std::string>& options) {
    m_pMenus->SetTitleMenu(menu, title.c_str());
    m_pMenus->SetBackMenu(menu, false);
    
    for (size_t i = 0; i < options.size(); ++i) {
        m_pMenus->AddItemMenu(menu, std::to_string(i).c_str(), options[i].c_str());
    }
    
    m_pMenus->SetCallback(menu, [this](const char* szItem, const char* szTranslate, int iItem, int iSlot) {
        this->OnVoteMenuCallback(szItem, szTranslate, iItem, iSlot);
    });
}

void CPugSystem::OnVoteMenuCallback(const char* szItem, const char* szTranslate, int iItem, int iSlot) {
    if (!m_bVoteInProgress) return;
    
    int vote = atoi(szItem);
    ProcessVote(iSlot, vote);
    m_pMenus->ClosePlayerMenu(iSlot);
}

void CPugSystem::StartTeamVote() {
    m_bVoteInProgress = true;
    m_votes.clear();
    m_voteTitle = "是否交换队伍?";
    
    std::vector<std::string> options = {"是", "否"};
    
    for (int i = 0; i < gpGlobals->maxClients; ++i) {
        CCSPlayerController* pPlayer = CCSPlayerController::FromSlot(i);
        if (pPlayer && pPlayer->IsController() && pPlayer->Get_Team() > 1) {
            ShowVoteMenu(i, m_voteTitle, options);
        }
    }
    
    m_nextFrameActions.push_back([this]() {
        gpGlobals->curtime + 10.0f, [this]() {
            this->CheckVoteResult();
        };
    });
}

void CPugSystem::ProcessVote(int slot, int vote) {
    if (m_votes.find(slot) != m_votes.end()) {
        CCSPlayerController* pPlayer = CCSPlayerController::FromSlot(slot);
        if (pPlayer) {
            pPlayer->ClientPrint(HUD_PRINTTALK, "[\x03NEKO PUG\x01] 你已经投过票了！");
        }
        return;
    }
    
    m_votes[slot] = vote;
    
    CCSPlayerController* pPlayer = CCSPlayerController::FromSlot(slot);
    if (pPlayer) {
        pPlayer->ClientPrint(HUD_PRINTTALK, "[\x03NEKO PUG\x01] 你的投票已记录！");
    }
}

void CPugSystem::CheckVoteResult() {
    if (!m_bVoteInProgress) return;
    
    int yes = 0, no = 0;
    for (const auto& vote : m_votes) {
        if (vote.second == 0) yes++;
        else no++;
    }
    
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "投票结果: 是 %d - 否 %d", yes, no);
    BroadcastMessage(buffer);
    
    if (yes > no) {
        SwapTeams();
        BroadcastMessage("投票通过，队伍已交换！");
    } else {
        BroadcastMessage("投票未通过，队伍保持不变！");
    }
    
    m_bVoteInProgress = false;
}

void CPugSystem::ShowMapVoteMenu() {
    m_bVoteInProgress = true;
    m_votes.clear();
    m_voteTitle = "请投票选择地图";
    
    for (int i = 0; i < gpGlobals->maxClients; ++i) {
        CCSPlayerController* pPlayer = CCSPlayerController::FromSlot(i);
        if (pPlayer && pPlayer->IsController()) {
            Menu menu;
            CreateMapVoteMenu(menu);
            m_pMenus->DisplayPlayerMenu(menu, i);
        }
    }
    
    m_nextFrameActions.push_back([this]() {
        gpGlobals->curtime + 15.0f, [this]() {
            this->CheckMapVoteResult();
        };
    });
}

void CPugSystem::CreateMapVoteMenu(Menu& menu) {
    m_pMenus->SetTitleMenu(menu, m_voteTitle.c_str());
    m_pMenus->SetBackMenu(menu, false);
    
    for (size_t i = 0; i < m_mapPool.size(); ++i) {
        m_pMenus->AddItemMenu(menu, std::to_string(i).c_str(), m_mapPool[i].c_str());
    }
    
    m_pMenus->SetCallback(menu, [this](const char* szItem, const char* szTranslate, int iItem, int iSlot) {
        this->OnMapVoteMenuCallback(szItem, szTranslate, iItem, iSlot);
    });
}

void CPugSystem::OnMapVoteMenuCallback(const char* szItem, const char* szTranslate, int iItem, int iSlot) {
    if (!m_bVoteInProgress) return;
    
    int vote = atoi(szItem);
    ProcessVote(iSlot, vote);
    m_pMenus->ClosePlayerMenu(iSlot);
}

void CPugSystem::CheckMapVoteResult() {
    if (!m_bVoteInProgress) return;
    
    std::unordered_map<int, int> voteCounts;
    
    for (const auto& vote : m_votes) {
        voteCounts[vote.second]++;
    }
    
    int winningMapIndex = 0;
    int maxVotes = 0;
    for (const auto& entry : voteCounts) {
        if (entry.second > maxVotes) {
            maxVotes = entry.second;
            winningMapIndex = entry.first;
        }
    }
    
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "投票结束，选择的地图是: %s", m_mapPool[winningMapIndex].c_str());
    BroadcastMessage(buffer);
    
    g_pEngineServer2->ChangeLevel(m_mapPool[winningMapIndex].c_str(), nullptr);
    
    m_bVoteInProgress = false;
}

void CPugSystem::StartMapVote() {
    if (m_matchState.Get() == MatchStateManager::State::WaitingForGameVote) {
        ShowMapVoteMenu();
    }
}

void CPugSystem::SelectCaptains() {
    m_captains.clear();
    
    float earliestTimeA = FLT_MAX, earliestTimeB = FLT_MAX;
    int captainA = -1, captainB = -1;
    
    for (int i = 0; i < gpGlobals->maxClients; ++i) {
        if (g_Player[i].isReady) {
            if (g_Player[i].readyTime < earliestTimeA) {
                earliestTimeB = earliestTimeA;
                captainB = captainA;
                earliestTimeA = g_Player[i].readyTime;
                captainA = i;
            } else if (g_Player[i].readyTime < earliestTimeB) {
                earliestTimeB = g_Player[i].readyTime;
                captainB = i;
            }
        }
    }
    
    if (captainA != -1) {
        g_Player[captainA].isCaptain = true;
        m_captains.push_back(captainA);
        
        CCSPlayerController* pCaptain = CCSPlayerController::FromSlot(captainA);
        if (pCaptain) {
            pCaptain->ClientPrint(HUD_PRINTTALK, "[\x03NEKO PUG\x01] 你被选为队长！");
            ShowCaptainMenu(captainA);
        }
    }
    
    if (captainB != -1) {
        g_Player[captainB].isCaptain = true;
        m_captains.push_back(captainB);
        
        CCSPlayerController* pCaptain = CCSPlayerController::FromSlot(captainB);
        if (pCaptain) {
            pCaptain->ClientPrint(HUD_PRINTTALK, "[\x03NEKO PUG\x01] 你被选为队长！");
            ShowCaptainMenu(captainB);
        }
    }
}

void CPugSystem::SelectPlayer(int captainSlot, const char* playerName) {
    CCSPlayerController* pCaptain = CCSPlayerController::FromSlot(captainSlot);
    if (!pCaptain || !pCaptain->IsController() || !g_Player[captainSlot].isCaptain) return;
    
    for (int i = 0; i < gpGlobals->maxClients; ++i) {
        CCSPlayerController* pPlayer = CCSPlayerController::FromSlot(i);
        if (pPlayer && pPlayer->IsController() && !strcmp(pPlayer->GetPlayerName(), playerName)) {
            TeamSide team = g_Player[captainSlot].team;
            pPlayer->SwitchTeam(team == TeamSide::TeamA ? 2 : 3);
            g_Player[i].team = team;
            
            char buffer[256];
            snprintf(buffer, sizeof(buffer), "[\x03NEKO PUG\x01] 玩家 %s 已被选入你的队伍！", playerName);
            pCaptain->ClientPrint(HUD_PRINTTALK, buffer);
            
            snprintf(buffer, sizeof(buffer), "[\x03NEKO PUG\x01] 你已被队长 %s 选中！", pCaptain->GetPlayerName());
            pPlayer->ClientPrint(HUD_PRINTTALK, buffer);
            
            return;
        }
    }
    
    pCaptain->ClientPrint(HUD_PRINTTALK, "[\x03NEKO PUG\x01] 找不到该玩家！");
}

void CPugSystem::AssignRandomTeams() {
    std::vector<int> readyPlayers;
    
    for (int i = 0; i < gpGlobals->maxClients; ++i) {
        if (g_Player[i].isReady && g_Player[i].team == TeamSide::None) {
            readyPlayers.push_back(i);
        }
    }
    
    std::shuffle(readyPlayers.begin(), readyPlayers.end(), m_rng);
    
    for (size_t i = 0; i < readyPlayers.size(); ++i) {
        int slot = readyPlayers[i];
        if (i < readyPlayers.size() / 2) {
            CCSPlayerController::FromSlot(slot)->SwitchTeam(2);
            g_Player[slot].team = TeamSide::TeamA;
        } else {
            CCSPlayerController::FromSlot(slot)->SwitchTeam(3);
            g_Player[slot].team = TeamSide::TeamB;
        }
    }
    
    BroadcastMessage("[\x03NEKO PUG\x01] 玩家已随机分配到两队！");
    m_matchState.Next();
}
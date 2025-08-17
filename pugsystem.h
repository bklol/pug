#pragma once

#include "neko_pug.h"
#include "GameMap.h"
#include "entity/ccsplayercontroller.h"
#include "PlayerMap.h"
#include "vendor/menus.h"
#include <functional>
#include <vector>
#include <string>
#include <unordered_map>
#include <random>

class CPugSystem {
public:
    void Init();
    void OnFrame();
    void OnPlayerCommand(CCSPlayerController* pController, const char* command);
    void OnPlayerJoin(int slot, uint64 xuid);
    void OnPlayerDisconnect(int slot);
    
    // 比赛控制
    void StartMatch();
    void EndMatch();
    void RestartMatch();
    
    // 玩家管理
    void SetPlayerReady(int slot, bool ready);
    void AssignTeams();
    void SwapTeams();
    void AssignRandomTeams();
    
    // 状态查询
    bool IsMatchLive() const;
    int GetReadyPlayerCount(int team = -1) const;
    int GetPlayerCount(int team = -1) const;
    MatchStateManager::State GetMatchState() const;

    // 菜单相关功能
    void ShowCaptainMenu(int slot);
    void ShowVoteMenu(int slot, const std::string& title, const std::vector<std::string>& options);
    void ShowMapVoteMenu();
    
    // 菜单回调处理
    void OnCaptainMenuCallback(const char* szItem, const char* szTranslate, int iItem, int iSlot);
    void OnVoteMenuCallback(const char* szItem, const char* szTranslate, int iItem, int iSlot);
    void OnMapVoteMenuCallback(const char* szItem, const char* szTranslate, int iItem, int iSlot);
    
    // 投票相关
    void StartTeamVote();
    void StartMapVote();
    void ProcessVote(int slot, int vote);
    void CheckVoteResult();
    void CheckMapVoteResult();
    
    // 队长功能
    void SelectCaptains();
    void SelectPlayer(int captainSlot, const char* playerName);
	
	void RequestPause(CCSPlayerController* pController);
    void RequestUnpause(CCSPlayerController* pController);
    bool IsMatchPaused() const;
private:
    void CheckReady();
    void StartVote();
    void StartKnifeRound();
    void StartLive();
    
    void ReadyHintMsg(CCSPlayerController* pController) const;
    void BroadcastMessage(const char* message) const;
    void TeamMessage(int team, const char* message) const;
    
    void UpdatePlayerTeamStatus();
    
    // 菜单创建函数
    void CreateCaptainMenu(Menu& menu);
    void CreateVoteMenu(Menu& menu, const std::string& title, const std::vector<std::string>& options);
    void CreateMapVoteMenu(Menu& menu);

    MatchStateManager m_matchState;
    std::vector<std::function<void()>> m_nextFrameActions;
	
	struct TeamPauseInfo {
        int remainingPauses;
        float lastPauseTime;
    };
    
    std::unordered_map<TeamSide, TeamPauseInfo> m_teamPauses;
    bool m_bMatchPaused;
    float m_flPauseStartTime;
    int m_iPausingTeam;
    std::unordered_map<int, bool> m_pauseVotes;
    
    void PauseMatch(TeamSide team);
    void UnpauseMatch();
	
    // 菜单相关成员
    IMenusApi* m_pMenus;
    std::unordered_map<int, int> m_votes;
    std::vector<std::string> m_mapPool;
    std::vector<int> m_captains;
    bool m_bVoteInProgress;
    std::string m_voteTitle;
    
    // 随机数生成器
    std::mt19937 m_rng;
};

extern CPugSystem g_PugSystem;
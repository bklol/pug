#pragma once
#include <unordered_map>

class MatchStateManager
{
public:
    enum State : int
    {
        None = 0,
        WarmUp,
        WaitingForGameVote,
        PickPlayers,        
        PickMap,
        WaitingForKnifeRound,
        KnifeRound,
        WaitingForKnifeRoundDecision,
        GoingLive,
        Live,
        MatchEnd
    };

private:
    State currentState;

public:
    MatchStateManager() : currentState(None) {}

    State Get() const { return currentState; }
    void Set(State newState) { currentState = newState; }

    bool IsLive() const { return currentState == Live; }
    bool IsVotingPhase() const { return currentState == WaitingForGameVote || currentState == PickMap; }
    bool IsFinished() const { return currentState == MatchEnd; }

    void Next() {
        if (currentState < MatchEnd)
            currentState = static_cast<State>(static_cast<int>(currentState) + 1);
    }

    const char* ToString() const {
        switch (currentState) {
            case None: return "无";
            case WarmUp: return "热身";
            case WaitingForGameVote: return "等待投票";
            case PickPlayers: return "选择玩家";            
            case PickMap: return "选择地图";
            case WaitingForKnifeRound: return "等待拼刀";
            case KnifeRound: return "拼刀";
            case WaitingForKnifeRoundDecision: return "等待选边";
            case GoingLive: return "比赛准备开始";
            case Live: return "比赛开始";
            case MatchEnd: return "比赛结束";
            default: return "Unknown";
        }
    }
};
#pragma once
#include <unordered_map>
#define MAXPLAYERS 64

enum class TeamSide
{
    None = -1,
    TeamA = 2,
    TeamB = 3
};

class Player
{
public:
    int playerId = -1;
    bool isReady = false;
    bool isInGame = false;
    bool isCaptain = false;
    float readyTime = 0.0f;
    TeamSide team = TeamSide::None;

    Player() = default;

    void Init()
    {
        playerId = -1;
        isReady = false;
        isInGame = false;
        isCaptain = false;
        readyTime = 0.0f;
        team = TeamSide::None;
    }
};

constexpr int MAX_PLAYERS = 64;
extern Player g_Player[MAX_PLAYERS + 1];
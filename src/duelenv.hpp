#pragma once

#include <random>

#include "player.hpp"

struct DuelAction
{
    PlayerInput input;
    Vector2 mouseDelta;
    bool fire = false;
};

struct DuelStepResult
{
    float player1Reward = 0.0f;
    float player2Reward = 0.0f;
    bool terminated = false;
    bool truncated = false;
};

class DuelEnvironment
{
public:
    float ArenaHalfSize = 10.0f;
    void Reset();
    void Reset(unsigned int seed);

    DuelEnvironment();
    explicit DuelEnvironment(unsigned int seed);
    Player& GetPlayer1();
    Player& GetPlayer2();

    DuelStepResult Step(
        DuelAction& player1Action,
        DuelAction& player2Action,
        float elapsedTime);

    float GetLerpAlpha();

private:
    
    double fixedDeltaTime = 1.0f/125.0f;
    Vector3 RandomSpawnPosition();

    Player player1;
    Player player2;
    double accumulatedTime = 0.0;
    std::mt19937 randomEngine;
    float episodeTime = 0.0f;
};

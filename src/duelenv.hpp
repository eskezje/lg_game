#pragma once

#include <random>

#include "player.hpp"

struct DuelAction
{
    PlayerInput input;
    Vector2 mouseDelta;
    bool fire = false;
};

class DuelEnvironment
{
public:
    float ArenaHalfSize = 10.0f;
    void Reset();

    DuelEnvironment();
    Player& GetPlayer1();
    Player& GetPlayer2();

    void Step(
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
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
};

#pragma once

#include <random>
#include <iostream>

#include "player.hpp"

struct DuelAction
{
    float forward = 0.0f;
    float sideward = 0.0f;
    float turn = 0.0f;
    float look = 0.0f;
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
        const DuelAction& player1Action,
        const DuelAction& player2Action,
        float elapsedTime);

private:

    Vector3 RandomSpawnPosition();

    Player player1;
    Player player2;
    double accumulatedTime = 0.0;
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
};

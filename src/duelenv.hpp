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

struct PlayerState
{
    Vector3 position;
    Vector3 velocity;
    Vector3 direction;
    float health = 0.0f;
    int maxHealth = 0;
    float gunCooldown = 0.0f;
};

struct DuelState
{
    PlayerState player1;
    PlayerState player2;

    float episodeTime = 0.0f;
    float episodeDuration = 20.0f;
    float arenaHalfSize = 0.0f;
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
    DuelState GetState() const;

private:
    
    double fixedDeltaTime = 1.0f/125.0f;
    Vector3 RandomSpawnPosition();

    Player player1;
    Player player2;
    double accumulatedTime = 0.0;
    std::mt19937 randomEngine;
    float episodeTime = 0.0f;
};

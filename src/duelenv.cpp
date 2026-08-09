#include "duelenv.hpp"

#include <random>
#include "raymath.h"

namespace
{
float spawnLimit = 9.0f;
float minimumSpawnDistance = 2.0f;
float pi = 3.14159265358979323846f;
}

void DuelEnvironment::Reset()
{
    // reset player positions to random places, they should not collide with eachother or be too close to eachother
    Vector3 spawnPosition1 = RandomSpawnPosition();
    Vector3 spawnPosition2 = RandomSpawnPosition();
    while (Vector3Distance(spawnPosition1, spawnPosition2) < minimumSpawnDistance)
    {
        spawnPosition2 = RandomSpawnPosition();
    }
    player1 = Player(spawnPosition1);
    player2 = Player(spawnPosition2);

    accumulatedTime = 0.0;

    // set player health to max
    player1.SetHealth(player1.GetMaxHealth());
    player2.SetHealth(player2.GetMaxHealth());
    // set players aim to random directions
}

DuelEnvironment::DuelEnvironment()
{
    Reset();
}

Player &DuelEnvironment::GetPlayer1()
{
    return player1;
}

Player &DuelEnvironment::GetPlayer2()
{
    return player2;
}

Vector3 DuelEnvironment::RandomSpawnPosition()
{   
    Vector3 spawnPosition;
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(-1.0, 1.0);
    spawnPosition.x = dis(gen) * spawnLimit;
    spawnPosition.z = dis(gen) * spawnLimit;
    spawnPosition.y = 0.0f;
    return spawnPosition;
}

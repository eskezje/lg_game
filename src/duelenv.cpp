#include "duelenv.hpp"

#include <random>
#include "raymath.h"

namespace
{
float spawnLimit = 9.0f;
float minimumSpawnDistance = 2.0f;
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
    episodeTime = 0.0f;
}

DuelStepResult DuelEnvironment::Step(DuelAction& player1Action, DuelAction& player2Action, float elapsedTime) {
    int player1HealthBefore = player1.GetHealth();
    int player2HealthBefore = player2.GetHealth();
    bool wasPlayer2Alive = player2HealthBefore > 0;
    
    player1.UpdateLook(player1Action.mouseDelta);
    player2.UpdateLook(player2Action.mouseDelta);
    accumulatedTime += elapsedTime;
    episodeTime += elapsedTime;
    while (accumulatedTime >= fixedDeltaTime) {
        // something eye position 
        player1.SetPrevEyePosition();
        player2.SetPrevEyePosition();

        // SimulateMovement
        player1.SimulateMovement(player1Action.input, (float)fixedDeltaTime, ArenaHalfSize);
        player2.SimulateMovement(player2Action.input, (float)fixedDeltaTime, ArenaHalfSize);
        // update collision
        player1.PlayerCollision(player2);
        //players shoot their guns
        player1.UpdateGun(player2, player1Action.fire, (float)fixedDeltaTime);
        player2.UpdateGun(player1, player2Action.fire, (float)fixedDeltaTime);
        accumulatedTime = accumulatedTime - fixedDeltaTime;
    }
    DuelStepResult result;
    int player1DamageDealt = player2HealthBefore-player2.GetHealth();
    float player1DamageReward = (float)(player1DamageDealt) /(float)(player2.GetMaxHealth());
    float player2SurvivalReward = 0.01f * elapsedTime;
    result.player1Reward = player1DamageReward - player2SurvivalReward;
    result.player2Reward = player2SurvivalReward - player1DamageReward;
    if (wasPlayer2Alive && player2.GetHealth() <= 0)
    {
        result.player1Reward += 1.0f;
        result.player2Reward -= 1.0f;
        result.terminated = true;
    }
    if (episodeTime >= 20.0f && !result.terminated)
    {
        result.player1Reward -= 1.0f;
        result.player2Reward += 1.0f;
        result.truncated = true;
    }
    return result;
    
    
    


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

float DuelEnvironment::GetLerpAlpha()
{
    return (float)(accumulatedTime/fixedDeltaTime);
}

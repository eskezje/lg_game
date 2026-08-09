#pragma once

#include "player.hpp"

class AIPlayer {
public:
    explicit AIPlayer(Vector3 spawnPosition);
    void Update(Player& enemy, float deltaTime, float arenaHalfSize);
    Player& GetCharacter();
    const Player& GetCharacter() const;
    void UpdateAim(Vector2 mouseDelta);

private:
    Player character;
    PlayerInput movementInput{};
};
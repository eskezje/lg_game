#include "ai.hpp"

AIPlayer::AIPlayer(Vector3 spawnPosition)
    : character(spawnPosition)
{
}

void AIPlayer::UpdateAim(Vector2 mouseDelta)
{
    character.UpdateLook(mouseDelta);
}

Player& AIPlayer::GetCharacter()
{
    return character;
}

const Player& AIPlayer::GetCharacter() const
{
    return character;
}
#include "player.hpp"

#include <algorithm>
#include <cmath>

#include <raymath.h>

Player::Player(
    Vector3 pos,
    float yaw,
    float pitch,
    float sensitivity,
    float radius,
    float height,
    float lookHeight,
    int health)

    : position(pos),
      yaw(yaw),
      pitch(pitch),
      sensitivity(sensitivity),
      playerRadius(radius),
      playerHeight(height),
      eyeHeight(lookHeight),
      playerHealth(health)
{
}

void Player::UpdateLook(Vector2 mouseDelta)
{
    yaw -= mouseDelta.x * sensitivity;
    pitch -= mouseDelta.y * sensitivity;

    constexpr float maxPitch = 1.55334f;

    pitch = std::clamp(pitch, -maxPitch, maxPitch);
}

void Player::SimulateMovement(const PlayerInput& input, float deltaTime, float arenaHalfSize)
{
    ApplyFriction(deltaTime);

    const float sinYaw = std::sin(yaw);
    const float cosYaw = std::cos(yaw);

    const Vector3 forward{sinYaw, 0.0f, cosYaw};

    const Vector3 right{-cosYaw, 0.0f, sinYaw};

    Vector3 wishVelocity = Vector3Add(
        Vector3Scale(forward, input.forward),
        Vector3Scale(right, input.sideward));

    const float wishLength = Vector3Length(wishVelocity);

    if (wishLength > 0.0001f)
    {
        const Vector3 wishDirection =Vector3Scale(wishVelocity, 1.0f / wishLength);

        const float inputMagnitude = std::min(wishLength, 1.0f);

        const float wishSpeed = MovementConfig::runSpeed * inputMagnitude;

        Accelerate(wishDirection, wishSpeed, MovementConfig::groundAcceleration, deltaTime);
    }

    const Vector3 potential_position = Vector3Add(position, Vector3Scale(velocity, deltaTime));

    const float playerLimit = arenaHalfSize - playerRadius;

    const float clampedX = std::clamp(potential_position.x, -playerLimit, playerLimit);
    const float clampedZ = std::clamp(potential_position.z, -playerLimit, playerLimit);
    if (clampedX != potential_position.x)
    {
        velocity.x = 0.0f;
    }
    if (clampedZ != potential_position.z)
    {
        velocity.z = 0.0f;
    }
    position.x = clampedX;
    position.z = clampedZ;
    
    // position = Vector3Add(position, Vector3Scale(velocity, deltaTime));
}

void Player::Accelerate(Vector3 wishDirection, float wishSpeed, float acceleration, float deltaTime)
{
    const float currentSpeed = Vector3DotProduct(velocity, wishDirection);

    const float additionalSpeed = wishSpeed - currentSpeed;

    if (additionalSpeed <= 0.0f)
    {
        return;
    }

    float accelerationSpeed = acceleration * wishSpeed * deltaTime;

    accelerationSpeed = std::min(accelerationSpeed, additionalSpeed);

    velocity = Vector3Add(velocity, Vector3Scale(wishDirection,accelerationSpeed));
}

void Player::ApplyFriction(float deltaTime)
{
    const float horizontalSpeed = std::sqrt(velocity.x * velocity.x + velocity.z * velocity.z);

    if (horizontalSpeed < 0.001f)
    {
        velocity.x = 0.0f;
        velocity.z = 0.0f;
        return;
    }

    const float control = std::max(horizontalSpeed, MovementConfig::stopSpeed);

    const float speedDrop = control * MovementConfig::groundFriction * deltaTime;

    const float newSpeed = std::max( horizontalSpeed - speedDrop,0.0f);

    const float scale = newSpeed / horizontalSpeed;

    velocity.x *= scale;
    velocity.z *= scale;
}

Vector3 Player::GetPosition() const
{
    return position;
}

Vector3 Player::GetDirection() const
{
    const float cosPitch = std::cos(pitch);

    return {
        cosPitch * std::sin(yaw),
        std::sin(pitch),
        cosPitch * std::cos(yaw)
    };
}

Vector3 Player::GetVelocity() const
{
    return velocity;
}

BoundingBox Player::GetHitbox() const
{
    return {
        Vector3{
            position.x - playerRadius,
            position.y,
            position.z - playerRadius
        },
        Vector3{
            position.x + playerRadius,
            position.y + playerHeight,
            position.z + playerRadius
        }
    };
}

Vector3 Player::GetEyePosition() const
{
    return Vector3{
        position.x,
        position.y + eyeHeight,
        position.z
    };
}

float Player::GetHeight() const
{
    return playerHeight;
}

float Player::GetRadius() const
{
    return playerRadius;
}

int Player::GetHealth() const
{
    return playerHealth;
}

bool Player::takeDamage(int damageAmount)
{
    playerHealth -= damageAmount;
    if (playerHealth <= 0)
    {
        playerHealth = 0;
        return true;
    }
    return false;
}

void Player::shoot(Player &secondPlayer)
{
    const Ray shot{ GetEyePosition(), Vector3Normalize(GetDirection())};
    const RayCollision hit = GetRayCollisionBox(shot, secondPlayer.GetHitbox());
    if (hit.hit)
    {
        bool killedEnemy = secondPlayer.takeDamage(1);
        HealPlayer(1);
    }
    
}

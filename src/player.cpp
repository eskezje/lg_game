#include "player.hpp"

#include <algorithm>
#include <cmath>

#include <raymath.h>

Player::Player(
    Vector3 pos,
    float yaw,
    float pitch,
    float sensitivity,
    float radius)

    : position(pos),
      yaw(yaw),
      pitch(pitch),
      sensitivity(sensitivity),
      playerRadius(radius)
{
}

void Player::UpdateLook(Vector2 mouseDelta)
{
    yaw -= mouseDelta.x * sensitivity;
    pitch -= mouseDelta.y * sensitivity;

    constexpr float maxPitch = 1.55334f;

    pitch = std::clamp(pitch, -maxPitch, maxPitch);
}

void Player::SimulateMovement(const PlayerInput& input, float deltaTime)
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

    const float arena_half_size = 10.0f;
    const float playerLimit = arena_half_size - playerRadius;

    if (potential_position.x > playerLimit)
    {
        position.x = playerLimit;
        if (velocity.x > 0.0f)
        {
            velocity.x = 0.0f;
        }
        
    }
    else if (potential_position.x < -playerLimit)
    {
        position.x = -playerLimit;
        if (velocity.x > 0.0f)
        {
            velocity.x = 0.0f;
        }
    }
    else
    {
        position.x = potential_position.x;
    }
    

    if (potential_position.z > playerLimit)
    {
        position.z = playerLimit;
        if (velocity.z > 0.0f)
        {
            velocity.z = 0.0f;
        }
        
    }
    else if (potential_position.z < -playerLimit)
    {
        position.x = -playerLimit;
        if (velocity.z > 0.0f)
        {
            velocity.z = 0.0f;
        }
        
    }
    else
    {
        position.z = potential_position.z;
    }
    
    

    if (potential_position.x < playerLimit && potential_position.x > -playerLimit)
    {
        position.x = potential_position.x;
    }
    else
    {
        position.x = std::clamp(potential_position.x, -playerLimit, playerLimit);
    }
    
    if (potential_position.z < playerLimit && potential_position.z > -playerLimit)
    {
        position.z = potential_position.z;
    }
    else
    {
        position.z = std::clamp(potential_position.z, -playerLimit, playerLimit);
    }
    
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
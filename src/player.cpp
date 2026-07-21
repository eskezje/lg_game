#include "player.hpp"

#include <raymath.h>
#include <algorithm>
#include <cmath>


Player::Player(
    Vector3 pos,
    float yaw,
    float pitch,
    float sensitivity,
    float speed
)
    : position(pos),
      yaw(yaw),
      pitch(pitch),
      sensitivity(sensitivity),
      speed(speed)
{
}

void Player::UpdateLook(Vector2 mouseDelta)
{
    yaw -= mouseDelta.x * sensitivity;
    pitch -= mouseDelta.y * sensitivity;

    constexpr float maxPitch = 1.55334f;

    pitch = std::clamp(pitch, -maxPitch, maxPitch);
}

void Player::SimulateMovement(const PlayerInput &input, float deltaTime)
{

    ApplyFriction(deltaTime);

    const float sinYaw = std::sin(yaw);
    const float cosYaw = std::cos(yaw);

    const Vector3 forward{ sinYaw, 0.0f, cosYaw};
    const Vector3 right{-cosYaw, 0.0f, sinYaw};

    Vector3 wishVelocity = Vector3Add(Vector3Scale(forward, input.forward), Vector3Scale(right, input.sideward));

    const float wishLength = Vector3Length(wishVelocity);

    if (wishLength > 0.0001f)
    {
        const Vector3 wishDirection = Vector3Scale(wishVelocity, 1.0f/wishLength);
        const float inputMagnitude = std::min(wishLength, 1.0f);
        const float wishSpeed = speed * inputMagnitude;
        constexpr float groundAcceleration = 10.0f;

        Accelerate(wishDirection, wishSpeed, groundAcceleration, deltaTime);
    }

    position = Vector3Add(position, Vector3Scale(velocity, deltaTime));
    
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

void Player::Accelerate(Vector3 wishDir, float wishSpeed, float acceleration, float deltaTime)
{
    const float currentSpeed = Vector3DotProduct(velocity, wishDir);
    const float additionalSpeed = wishSpeed - currentSpeed;
    if (additionalSpeed <= 0.0f)
    {
        return;
    }
    
    float accelerationSpeed = acceleration * wishSpeed * deltaTime;
    accelerationSpeed = std::min(accelerationSpeed, additionalSpeed);
    velocity = Vector3Add(velocity, Vector3Scale(wishDir, accelerationSpeed));
}

void Player::ApplyFriction(float detlaTime)
{
    const float horizontalSpeed = std::sqrt( velocity.x * velocity.x + velocity.z * velocity.z);
    if (horizontalSpeed < 0.001f)
    {
        velocity.x = 0.0f;
        velocity.z = 0.0f;
        return;
    }
    constexpr float friction = 6.0f;
    constexpr float stopSpeed = 3.125f;

    const float control = std::max(horizontalSpeed, stopSpeed);
    const float speedDrop = control * friction * detlaTime;
    const float newSpeed = std::max(horizontalSpeed - speedDrop, 0.0f);
    const float scale = newSpeed / horizontalSpeed;
    velocity.x *= scale;
    velocity.z *= scale;
}

Vector3 Player::GetVelocity() const
{
    return velocity;
}
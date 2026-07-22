#pragma once

#include <raylib.h>

struct PlayerInput
{
    float forward = 0.0f;
    float sideward = 0.0f;
};

struct MovementConfig
{
    static constexpr float runSpeed = 320.0f / 32.0f;
    static constexpr float stopSpeed = 100.0f / 32.0f;

    static constexpr float groundAcceleration = 10.0f;
    static constexpr float airAcceleration = 1.0f;
    static constexpr float groundFriction = 6.0f;

    static constexpr float gravity = 800.0f / 32.0f;
    static constexpr float jumpSpeed = 270.0f / 32.0f;
};

class Player
{
public:
    Player(
        Vector3 pos = {1.5f, 0.5f, 1.5f},
        float yaw = 0.0f,
        float pitch = 0.0f,
        float sensitivity = 0.003f,
        float radius = 0.4f);

    void UpdateLook(Vector2 mouseDelta);

    void SimulateMovement(const PlayerInput& input, float deltaTime);

    Vector3 GetPosition() const;
    Vector3 GetDirection() const;
    Vector3 GetVelocity() const;

private:
    void Accelerate(
        Vector3 wishDirection,
        float wishSpeed,
        float acceleration,
        float deltaTime);   

    void ApplyFriction(float deltaTime);

    Vector3 position{};
    Vector3 velocity{};

    float yaw;
    float pitch;
    float sensitivity;
    float playerRadius;
};
#pragma once

#include <raylib.h>

struct PlayerInput
{
    float forward = 0.0f;
    float sideward = 0.0f;
};

class Player
{
public:
    Player(
        Vector3 pos = {1.5f, 0.5f, 1.5f},
        float yaw = 0.0f,
        float pitch = 0.0f,
        float sensitivity = 0.003f,
        float speed = 2.0f
    );

    void UpdateLook(Vector2 mouseDelta);
    void SimulateMovement(const PlayerInput& input, float deltaTime);

    Vector3 GetPosition() const;
    Vector3 GetDirection() const;
    Vector3 GetVelocity() const;

private:
    void Accelerate(Vector3 wishDir, float wishSpeed, float acceleration, float deltaTime);
    void ApplyFriction(float detlaTime);

    Vector3 position{};
    Vector3 velocity{};

    float yaw;
    float pitch;
    float sensitivity;
    float speed;
};
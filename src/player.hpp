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

struct GunConfig
{
    static constexpr int damage = 4;
    static constexpr float tickRate = 40.0f;
    static constexpr int healingOnHit = 4;
};

class Player
{
public:
    Player(
        Vector3 pos = {1.5f, 0.0f, 1.5f},
        float yaw = 0.0f,
        float pitch = 0.0f,
        float sensitivity = 0.003f,
        float radius = 0.4f,
        float height = 1.8f,
        float lookHeight = 1.6f,
        int health = 400);

    void UpdateLook(Vector2 mouseDelta);

    void SimulateMovement(const PlayerInput& input, float deltaTime, float arenaHalfSize);

    Vector3 GetPosition() const;
    Vector3 GetDirection() const;
    Vector3 GetVelocity() const;

    BoundingBox GetHitbox() const;

    Vector3 GetEyePosition() const;
    float GetHeight() const;
    float GetRadius() const;
    int GetHealth() const;
    bool takeDamage(int damageAmount);
    void UpdateGun(Player &secondPlayer, bool triggerHeld, float deltaTime);
    void shoot(Player &secondPlayer);
    void PlayerCollision(Player & secondPlayer);
    void SetHealth(int health);
    int GetMaxHealth() const;
    float GetGunCooldown() const;
    void SetPrevEyePosition();
    Vector3 GetPrevEyePosition();

private:
    void Accelerate(
        Vector3 wishDirection,
        float wishSpeed,
        float acceleration,
        float deltaTime);   

    void ApplyFriction(float deltaTime);
    void HealPlayer(int healingAmount);

    Vector3 position{};
    Vector3 velocity{};

    Vector3 prevEyePosition{};

    float yaw;
    float pitch;
    float sensitivity;
    float playerRadius;
    float playerHeight;
    float eyeHeight;
    float gunCooldown{};
    int maxPlayerHealth;
    int playerHealth;


};

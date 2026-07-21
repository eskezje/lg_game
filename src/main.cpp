#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>
#include <cmath>

#include "player.hpp"

PlayerInput ReadPlayerInputs()
{
    PlayerInput input;

    input.forward = static_cast<float>(IsKeyDown(KEY_W)) -
                    static_cast<float>(IsKeyDown(KEY_S));

    input.sideward = static_cast<float>(IsKeyDown(KEY_D)) -
                     static_cast<float>(IsKeyDown(KEY_A));

    return input;
}

int main()
{
    InitWindow(800, 600, "lg Game");
    DisableCursor();
    Player player;

    rlSetLineWidth(3.0f);
    Camera3D camera = {};
    camera.position = Vector3{0.0f, 0.5f, 0.0f};
    camera.target = Vector3{1.0f, 0.5f, 1.0f};
    camera.up = Vector3{0.0f, 1.0f, 0.0f};
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    float mapsize = 10.0f;

    while (WindowShouldClose() == false)
    {
        const PlayerInput input = ReadPlayerInputs();

        player.UpdateLook(GetMouseDelta());

        player.SimulateMovement(input, GetFrameTime());

        const Vector3 playerPosition = player.GetPosition();

        const Vector3 playerDirection = player.GetDirection();

        camera.position = playerPosition;
        camera.target = Vector3Add(playerPosition, playerDirection);

        BeginDrawing();

        ClearBackground(SKYBLUE);
        BeginMode3D(camera);

        DrawPlane(Vector3{0.0f, -0.01f, 0.0f}, Vector2{mapsize * 2.0f, mapsize * 2.0f}, WHITE);
        DrawGrid(mapsize * mapsize, 0.2f);

        EndMode3D();
        
        DrawFPS(10, 10);
        const Vector3 velocity = player.GetVelocity();
        const float horizontalSpeed = std::sqrt(velocity.x * velocity.x + velocity.z * velocity.z);
        DrawText(
        TextFormat("Speed: %.2f", horizontalSpeed), 10, 35, 20, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>
#include <cmath>
#include <algorithm>

#include "player.hpp"

PlayerInput ReadPlayerInputs()
{
    PlayerInput input;

    input.forward = IsKeyDown(KEY_W) - IsKeyDown(KEY_S);

    input.sideward = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);

    return input;
}

int main()
{
    InitWindow(800, 600, "lg Game");
    DisableCursor();

    SetExitKey(KEY_NULL);       // Disable KEY_ESCAPE to close window, X-button still works

    bool exitWindowRequested = false;   // Flag to request window to exit
    bool exitWindow = false;    // Flag to set window to exit

    Player player;

    rlSetLineWidth(3.0f);
    Camera3D camera = {};
    camera.position = Vector3{0.0f, 0.5f, 0.0f};
    camera.target = Vector3{1.0f, 0.5f, 1.0f};
    camera.up = Vector3{0.0f, 1.0f, 0.0f};
    camera.fovy = 90.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    float mapsize = 10.0f;

    double accumulation = 0.0f;
    constexpr double fixedDeltaTime = 1.0f/125.0f;

    while (!exitWindow)
    {
        if (WindowShouldClose() || IsKeyPressed(KEY_ESCAPE)) exitWindowRequested = true;

        if (exitWindowRequested)
        {
            if (IsKeyPressed(KEY_Y)) exitWindow = true;
            else if (IsKeyPressed(KEY_N)) exitWindowRequested = false;
            
        }
        const float frameTime = std::min(GetFrameTime(),0.25f);

        accumulation = accumulation + frameTime;

        const PlayerInput input = ReadPlayerInputs();

        player.UpdateLook(GetMouseDelta());

        if (accumulation >= fixedDeltaTime)
        {
            player.SimulateMovement(input, static_cast<float>(fixedDeltaTime));

            accumulation = accumulation - fixedDeltaTime;
        }

        const Vector3 playerPosition = player.GetPosition();

        const Vector3 playerDirection = player.GetDirection();

        camera.position = playerPosition;
        camera.target = Vector3Add(playerPosition, playerDirection);

        BeginDrawing();
        
        ClearBackground(SKYBLUE);
        BeginMode3D(camera);

        DrawPlane(Vector3{0.0f, -0.01f, 0.0f}, Vector2{mapsize * 2.0f, mapsize * 2.0f}, WHITE);
        DrawGrid(mapsize * mapsize, 1.0f);

        EndMode3D();
        
        DrawFPS(10, 10);
        const Vector3 velocity = player.GetVelocity();
        const float horizontalSpeed = std::sqrt(velocity.x * velocity.x + velocity.z * velocity.z);
        DrawText(
        TextFormat("Speed: %.2f", horizontalSpeed), 10, 35, 20, BLACK);
        DrawText(TextFormat("Position x:%.2f, y:%.2f, z:%.2f", playerPosition.x, playerPosition.y, playerPosition.z), 10, 55, 20, BLACK); 

        if (exitWindowRequested)
        {
            DrawRectangle(0, 100, GetScreenWidth(), 200, BLACK);
            DrawText("Are you sure you want to exit program? [Y/N]", 40, 180, 30, WHITE);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

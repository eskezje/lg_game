#include <raylib.h>
#include "rlgl.h"
#include <raymath.h>
#include <cmath>
#include <algorithm>

#include "player.hpp"
#include "wall_render.hpp"
#include "renderer.hpp"

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

    Player firstPlayer;
    Player secondPlayer(Vector3{1.5f, 0.0f, -1.5f});

    float mapsize = 10.0f;
    float wallLength = mapsize*2.0f;
    float wallHeight = 10.0f;
    float wallDepth = 0.1f;

    Renderer test_renderer;
    test_renderer.Initialize(wallLength, wallHeight, wallDepth, mapsize);

    double accumulation = 0.0f;
    constexpr double fixedDeltaTime = 1.0f/125.0f;

    while (!exitWindow)
    {
        if (WindowShouldClose() || IsKeyPressed(KEY_ESCAPE)) {
            exitWindowRequested = true;
        }
        
        if (exitWindowRequested)
        {
            if (IsKeyPressed(KEY_Y)) exitWindow = true;
            else if (IsKeyPressed(KEY_N)) exitWindowRequested = false;
            
        }

        const float frameTime = std::min(GetFrameTime(),0.25f);

        accumulation = accumulation + frameTime;

        const PlayerInput input = ReadPlayerInputs();

        firstPlayer.UpdateLook(GetMouseDelta());

        if (accumulation >= fixedDeltaTime)
        {
            firstPlayer.SimulateMovement(input, (float)(fixedDeltaTime));
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                firstPlayer.shoot(secondPlayer);
            }
            

            accumulation = accumulation - fixedDeltaTime;
        }

        const Vector3 playerPosition = firstPlayer.GetPosition();
        const Vector3 playerDirection = firstPlayer.GetDirection();
        
        test_renderer.UpdateCameraPosTar(firstPlayer.GetEyePosition(), firstPlayer.GetDirection());
        

        BeginDrawing();
        
        ClearBackground(SKYBLUE);
        BeginMode3D(test_renderer.GetCamera());

        test_renderer.Render();
        test_renderer.RenderPlayer(secondPlayer);
        
        EndMode3D();

        test_renderer.RenderPlayerHealth(secondPlayer);
        
        DrawFPS(10, 10);
        const Vector3 velocity = firstPlayer.GetVelocity();
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

    test_renderer.Shutdown();
    CloseWindow();
    return 0;
}

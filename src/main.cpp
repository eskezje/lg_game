#include <raylib.h>
#include "rlgl.h"
#include <raymath.h>
#include <cmath>
#include <algorithm>

#include "player.hpp"
#include "wall_render.hpp"
#include "renderer.hpp"
#include "duelenv.hpp"

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

    DuelEnvironment duelEnv;

    Player firstPlayer = duelEnv.GetPlayer1();
    Player secondPlayer = duelEnv.GetPlayer2();

    float arenaHalfSize = duelEnv.ArenaHalfSize;
    float arenaWidth = arenaHalfSize * 2.0f;
    float wallHeight = 10.0f;
    float wallDepth = 0.1f;

    Renderer test_renderer;
    test_renderer.Initialize(arenaWidth, wallHeight, wallDepth, arenaHalfSize);

    double accumulation = 0.0f;
    constexpr double fixedDeltaTime = 1.0f/125.0f;

    Vector3 previousEyePosition = firstPlayer.GetEyePosition();

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
        const bool triggerHeld = IsMouseButtonDown(MOUSE_BUTTON_LEFT);

        firstPlayer.UpdateLook(GetMouseDelta());

        while (accumulation >= fixedDeltaTime)
        {
            previousEyePosition = firstPlayer.GetEyePosition();
            firstPlayer.SimulateMovement(input, (float)(fixedDeltaTime), arenaHalfSize);
            firstPlayer.PlayerCollision(secondPlayer);
            firstPlayer.UpdateGun(secondPlayer, triggerHeld, (float)(fixedDeltaTime));
            

            accumulation = accumulation - fixedDeltaTime;
        }
        float alpha = (float)(accumulation / fixedDeltaTime);
        Vector3 renderEyePosition = Vector3Lerp(previousEyePosition, firstPlayer.GetEyePosition(), alpha);
        

        const Vector3 playerPosition = firstPlayer.GetPosition();
        // const Vector3 playerDirection = firstPlayer.GetDirection();
        
        test_renderer.UpdateCameraPosTar(renderEyePosition, firstPlayer.GetDirection());
        

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

#include <raylib.h>
#include "rlgl.h"
#include <raymath.h>
#include <cmath>
#include <algorithm>

#include "player.hpp"
#include "wall_render.hpp"
#include "render.hpp"

const Vector3 cube_positions[] = {
                            {0.0f, 0.0f, 10.06f},
                            {0.0f, 0.0f, -10.06f},
                            {10.06f, 0.0f, 0.0f},
                            {-10.06f, 0.0f, 0.0f}};

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

    Camera3D camera = {};
    Texture2D wall0Texture{};
    Texture2D wall1Texture{};
    Texture2D wall2Texture{};
    Texture2D wall3Texture{};

    render::Initialize(camera, wall0Texture, wall1Texture, wall2Texture, wall3Texture);

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

        DrawPlane(Vector3{0.0f, -0.01f, 0.0f}, Vector2{mapsize * 4.0f, mapsize * 4.0f}, WHITE);
        DrawGrid(mapsize * mapsize, 0.2f);

        constexpr float wallLength = 20.0f;
        constexpr float wallHeight = 10.0f;
        constexpr float wallDepth = 0.1f;

        const Rectangle wall0Source = Rectangle{0.0f, 0.0f, (float)(wall0Texture.width), (float)(wall0Texture.height)};
        const Rectangle wall1Source = Rectangle{0.0f, 0.0f, (float)(wall1Texture.width), (float)(wall1Texture.height)};
        const Rectangle wall2Source = Rectangle{0.0f, 0.0f, (float)(wall2Texture.width), (float)(wall2Texture.height)};
        const Rectangle wall3Source = Rectangle{0.0f, 0.0f, (float)(wall3Texture.width), (float)(wall3Texture.height)};

        DrawCubeTextureRec(wall0Texture, wall0Source, cube_positions[0], wallLength, wallHeight, wallDepth, WHITE);
        DrawCubeTextureRec(wall1Texture, wall1Source, cube_positions[1], wallLength, wallHeight, wallDepth, WHITE);
        DrawCubeTextureRec(wall2Texture, wall2Source, cube_positions[2], wallDepth, wallHeight, wallLength, WHITE);
        DrawCubeTextureRec(wall3Texture, wall3Source, cube_positions[3], wallDepth, wallHeight, wallLength, WHITE);
        
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

    UnloadTexture(wall0Texture);
    UnloadTexture(wall1Texture);
    UnloadTexture(wall2Texture);
    UnloadTexture(wall3Texture);
    CloseWindow();
    return 0;
}

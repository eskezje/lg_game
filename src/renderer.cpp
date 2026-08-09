#include "renderer.hpp"
#include <raylib.h>
#include "raymath.h"
#include "wall_render.hpp"
#include <array>

void Renderer::Initialize(float wallLength, float wallHeight, float wallDepth, float map_size) {
    std::array<Image, 4> all_images{};
    all_images[0] = GenImageGradientLinear(512, 512, 0, DARKBLUE, SKYBLUE);
    all_images[1] = GenImageGradientRadial(512, 512, 0.8f, DARKGREEN, GREEN);
    all_images[2] = GenImageGradientSquare(512, 512, 0.75f, ORANGE, RED);
    all_images[3] = GenImageGradientLinear(512, 512, 90, PURPLE, PINK);
    
    for (size_t i = 0; i < 4; i++)
    {
        wallTextures[i] = LoadTextureFromImage(all_images[i]);
        UnloadImage(all_images[i]);
    }

    for (int i = 0; i < 4; i++)
    {
        if (wallTextures[i].id == 0)
        {
            TraceLog(LOG_ERROR, "Failed to create one or more wall textures from generated images");
        }
        
    }

    camera.position = Vector3{0.0f, 0.5f, 0.0f};
    camera.target = Vector3{1.0f, 0.5f, 1.0f};
    camera.up = Vector3{0.0f, 1.0f, 0.0f};
    camera.fovy = 90.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    CreateCubeWalls(wallLength, wallHeight, wallDepth);

    mapSize = map_size;

}

void Renderer::Shutdown()
{
    UnloadRendererTexture();
}

Camera3D Renderer::GetCamera(){
    return camera;
}

void Renderer::UpdateCameraPosTar(Vector3 position, Vector3 direction){
    camera.position = position;
    camera.target = Vector3Add(position, direction);
}

void Renderer::UnloadRendererTexture()
{
    for (size_t i = 0; i < 4; i++)
    {
        UnloadTexture(wallTextures[i]);
    }
}

void Renderer::DrawGridPlane(float mapsize)
{
    DrawPlane(Vector3{0.0f, -0.01f, 0.0f}, Vector2{mapsize * 4.0f, mapsize * 4.0f}, WHITE);
    DrawGrid(mapsize * mapsize, 0.2f);
}

void Renderer::CreateCubeWalls(float length, float height, float depth)
{
    wallLength = length;
    wallHeight = height;
    wallDepth = depth;

    float offset = wallLength / 2.0f + wallDepth / 2.0f;

    cubePositions = {
        Vector3{0.0f, 0.0f, offset},
        Vector3{0.0f, 0.0f, -offset},
        Vector3{offset, 0.0f, 0.0f},
        Vector3{-offset, 0.0f, 0.0f}
    };

    for (size_t i = 0; i < 4; i++)
    {
        wallSources[i] = Rectangle{0.0f, 0.0f, (float)(wallTextures[i].width), (float)(wallTextures[i].height)};
    }
    
}

void Renderer::DrawWalls()
{
    for (size_t i = 0; i < 4; i++)
    {
        bool wallOrientation = i < 2;
        float width = wallOrientation ? wallLength : wallDepth;
        float depth = wallOrientation ? wallDepth : wallLength;
        DrawCubeTextureRec(wallTextures[i], wallSources[i], cubePositions[i], width, wallHeight, depth, WHITE);
        
    }
    
    // DrawCubeTextureRec(wallTextures[0], wallSources[0], cubePositions[0], wallLength, wallHeight, wallDepth, WHITE);
    // DrawCubeTextureRec(wallTextures[1], wallSources[1], cubePositions[1], wallLength, wallHeight, wallDepth, WHITE);
    // DrawCubeTextureRec(wallTextures[2], wallSources[2], cubePositions[2], wallDepth, wallHeight, wallLength, WHITE);
    // DrawCubeTextureRec(wallTextures[3], wallSources[3], cubePositions[3], wallDepth, wallHeight, wallLength, WHITE);
}


void Renderer::Render()
{
    DrawGridPlane(mapSize);
    DrawWalls();
}

void Renderer::RenderPlayer(Player &player)
{
    Vector3 bottom = player.GetPosition();
    Vector3 top = Vector3{
        bottom.x,
        bottom.y + player.GetHeight(),
        bottom.z
    };

    DrawCylinderEx(bottom, top, player.GetRadius(), player.GetRadius(), 16, RED);
}

void Renderer::RenderPlayerHealth(Player &player)
{
    Vector3 textPosition = player.GetPosition();
    textPosition.y += player.GetHeight() + 0.25f;

    const Vector2 screenPosition = GetWorldToScreen(textPosition, camera);
    const char* healthText = TextFormat("%d HP", player.GetHealth());
    constexpr int fontSize = 20;
    const int textWidth = MeasureText(healthText, fontSize);

    DrawText(
        healthText,
        static_cast<int>(screenPosition.x - textWidth / 2.0f),
        static_cast<int>(screenPosition.y),
        fontSize,
        RED);
}

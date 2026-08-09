#include "renderer.hpp"
#include <raylib.h>
#include "raymath.h"
#include "wall_render.hpp"

void Renderer::Initialize(float wallLength, float wallHeight, float wallDepth, float map_size) {
    Image wall0Image = GenImageGradientLinear(512, 512, 0, DARKBLUE, SKYBLUE);
    Image wall1Image = GenImageGradientRadial(512, 512, 0.8f, DARKGREEN, GREEN);
    Image wall2Image = GenImageGradientSquare(512, 512, 0.75f, ORANGE, RED);
    Image wall3Image = GenImageGradientLinear(512, 512, 90, PURPLE, PINK);
    
    wallTextures[0] = LoadTextureFromImage(wall0Image);
    wallTextures[1] = LoadTextureFromImage(wall1Image);
    wallTextures[2] = LoadTextureFromImage(wall2Image);
    wallTextures[3] = LoadTextureFromImage(wall3Image);
    
    UnloadImage(wall0Image);
    UnloadImage(wall1Image);
    UnloadImage(wall2Image);
    UnloadImage(wall3Image);

    if (wallTextures[0].id == 0 || wallTextures[1].id == 0 || wallTextures[2].id == 0 || wallTextures[3].id == 0)
    {
        TraceLog(LOG_ERROR, "Failed to create one or more wall textures from generated images");
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
    UnloadTexture(wallTextures[0]);
    UnloadTexture(wallTextures[1]);
    UnloadTexture(wallTextures[2]);
    UnloadTexture(wallTextures[3]);
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
}

void Renderer::DrawWalls()
{
    wallSources[0] = Rectangle{0.0f, 0.0f, (float)(wallTextures[0].width), (float)(wallTextures[0].height)};
    wallSources[1] = Rectangle{0.0f, 0.0f, (float)(wallTextures[1].width), (float)(wallTextures[1].height)};
    wallSources[2] = Rectangle{0.0f, 0.0f, (float)(wallTextures[2].width), (float)(wallTextures[2].height)};
    wallSources[3] = Rectangle{0.0f, 0.0f, (float)(wallTextures[3].width), (float)(wallTextures[3].height)};

    DrawCubeTextureRec(wallTextures[0], wallSources[0], cubePositions[0], wallLength, wallHeight, wallDepth, WHITE);
    DrawCubeTextureRec(wallTextures[1], wallSources[1], cubePositions[1], wallLength, wallHeight, wallDepth, WHITE);
    DrawCubeTextureRec(wallTextures[2], wallSources[2], cubePositions[2], wallDepth, wallHeight, wallLength, WHITE);
    DrawCubeTextureRec(wallTextures[3], wallSources[3], cubePositions[3], wallDepth, wallHeight, wallLength, WHITE);
}


void Renderer::Render()
{
    DrawGridPlane(mapSize);
    DrawWalls();
}
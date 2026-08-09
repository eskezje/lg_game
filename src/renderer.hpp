#pragma once
#include <raylib.h>
#include <array>
#include "player.hpp"

class Renderer
{
public:
    void Initialize(float wallLength, float wallHeight, float wallDepth, float arenaHalfSize);
    void Shutdown();
    Camera3D GetCamera();
    void UpdateCameraPosTar(Vector3 position, Vector3 direction);
    void Render();
    void RenderPlayer(Player &player);
    void RenderPlayerHealth(Player &player);

private:
    void DrawGridPlane(float arenaHalfSize);
    void DrawWalls();
    void CreateCubeWalls(float wallLength, float wallHeight, float wallDepth);
    std::array<Vector3, 4> cubePositions{};
    float wallLength{};
    float wallHeight{};
    float wallDepth{};
    float arenaHalfSize{};
    Camera3D camera{};
    //Texture2D wallTextures[4]{};
    Texture2D wallTexture;
    Rectangle wallSources[4]{};
    void UnloadRendererTexture();
};

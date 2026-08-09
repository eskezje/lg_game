#pragma once
#include <raylib.h>
#include <array>

class Renderer
{
public:
    void Initialize(float wallLength, float wallHeight, float wallDepth, float mapSize);
    void Shutdown();
    Camera3D GetCamera();
    void UpdateCameraPosTar(Vector3 position, Vector3 direction);
    void Render();

private:
    void DrawGridPlane(float mapsize);
    void DrawWalls();
    void CreateCubeWalls(float wallLength, float wallHeight, float wallDepth);
    std::array<Vector3, 4> cubePositions{};
    float wallLength{};
    float wallHeight{};
    float wallDepth{};
    float mapSize{};
    Camera3D camera{};
    Texture2D wallTextures[4]{};
    Rectangle wallSources[4]{};
    void UnloadRendererTexture();
    bool IsWallInFront(Vector3 wallPosition);
};

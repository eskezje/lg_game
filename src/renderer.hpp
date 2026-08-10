#pragma once
#include <raylib.h>
#include <array>
#include "player.hpp"

enum class RenderCameraMode
{
    Player1,
    Player2,
    TopDown
};

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
    void UpdateCamera(RenderCameraMode mode, Player& player1, Player& player2, float interpolationAlpha);

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

#pragma once

#include <raylib.h>

void DrawWallOutlineZ(
    Vector3 position,
    float width,
    float height,
    float wallDepth,
    float outlineThickness,
    Color color);

void DrawWallOutlineX(
    Vector3 position,
    float wallWidth,
    float height,
    float depth,
    float outlineThickness,
    Color color);

void DrawCubeTexture(Texture2D texture, Vector3 position, float width, float height, float length, Color color);

void DrawCubeTextureRec(Texture2D texture, Rectangle source, Vector3 position, float width, float height, float length, Color color);
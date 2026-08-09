#pragma once
#include <raylib.h>

namespace render
{
    void Initialize(
        Camera3D& camera,
        Texture2D& wall0Texture,
        Texture2D& wall1Texture,
        Texture2D& wall2Texture,
        Texture2D& wall3Texture
    );
}
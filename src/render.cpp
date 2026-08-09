#include "render.hpp"
#include <raylib.h>

namespace render 
{
    void Initialize(
            Camera3D& camera, 
            Texture2D& wall0Texture,
            Texture2D& wall1Texture,
            Texture2D& wall2Texture,
            Texture2D& wall3Texture)
    {

        Image wall0Image = GenImageGradientLinear(512, 512, 0, DARKBLUE, SKYBLUE);
        Image wall1Image = GenImageGradientRadial(512, 512, 0.8f, DARKGREEN, GREEN);
        Image wall2Image = GenImageGradientSquare(512, 512, 0.75f, ORANGE, RED);
        Image wall3Image = GenImageGradientLinear(512, 512, 90, PURPLE, PINK);

        wall0Texture = LoadTextureFromImage(wall0Image);
        wall1Texture = LoadTextureFromImage(wall1Image);
        wall2Texture = LoadTextureFromImage(wall2Image);
        wall3Texture = LoadTextureFromImage(wall3Image);

        UnloadImage(wall0Image);
        UnloadImage(wall1Image);
        UnloadImage(wall2Image);
        UnloadImage(wall3Image);

        if (wall0Texture.id == 0 || wall1Texture.id == 0 || wall2Texture.id == 0 || wall3Texture.id == 0)
        {
            TraceLog(LOG_ERROR, "Failed to create one or more wall textures from generated images");
        }

        camera.position = Vector3{0.0f, 0.5f, 0.0f};
        camera.target = Vector3{1.0f, 0.5f, 1.0f};
        camera.up = Vector3{0.0f, 1.0f, 0.0f};
        camera.fovy = 90.0f;
        camera.projection = CAMERA_PERSPECTIVE;

    }
}
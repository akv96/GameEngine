#include "main.h"

GAME_UPDATE_AND_RENDER(GameUpdateAndRender)
{
    platform_controller *Keyboard = &Input->Controller[0];
    //DrawPixel(Video, Keyboard->MouseX, Keyboard->MouseY, 0xFFFFFFFF);
    rgba Color = {1.0f};
    //DrawBresenhamLine(Video, 0, 0, 100, 100, &Color);

    f32 Angle = 0;
    f32 FieldOfView = 90;
    f32 AspectRatio = (f32)Video->Width / (f32)Video->Height;
    f32 NearPlane = 0.1f;
    f32 FarPlane = 1000.0f;
    matrix4x4 ProjectionMatrix = {0};
    MatrixPerspective(&ProjectionMatrix, FieldOfView, AspectRatio, NearPlane, FarPlane);

    triangle Triangle = {{{1.0f, 1.0f, 3.0f}, {-1.0f, 1.0f, 3.0f}, {0.0f, -1.0f, 3.0f}}};
    ProjectAndDrawTriangle(Video, &Triangle);
}
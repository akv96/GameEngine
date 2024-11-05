#include "main.hpp"

GAME_UPDATE_AND_RENDER(GameUpdateAndRender)
{
    rectangle Rectangle = {0, 0, 100, 100};
    rgba Color = {1.0f};
    //DrawRectangle(Video, &Rectangle, &Color);
    //DrawPixel(Video, 10, 10, 0xFFFFFFFF);
    platform_controller *Keyboard = &Input->Controller[0];
    //DrawBresenhamLine(Video, Keyboard->MouseX, Video->Height - Keyboard->MouseY, Rectangle.Width, Rectangle.Height, &Color);    
}
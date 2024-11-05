#include "video.hpp"

internal_function void
DrawPixel(platform_video *Video, int X, int Y, u32 Color)
{
    if(X < 0)
    {
        X = 0;
    }
    else if(X >= Video->Width)
    {
        X = Video->Width - 1;
    }

    if(Y < 0)
    {
        Y = 0;
    }
    else if(Y >= Video->Height)
    {
        Y = Video->Height - 1;
    }

    u8 *Row = (u8 *)Video->Memory + (X * Video->BytesPerPixel) + (Y * Video->Pitch);
    u32 *Pixel = (u32 *)Row;
    *Pixel = Color;
}

internal_function void
DrawPixelFromRGBA(platform_video *Video, int X, int Y, rgba *Color)
{
    u8 R = (u8)(Color->R * 255.0f);
    u8 G = (u8)(Color->G * 255.0f);
    u8 B = (u8)(Color->B * 255.0f);
    u8 A = (u8)(Color->A * 255.0f);

    u32 ColorU32 = (A << 24) | (R << 16) | (G << 8) | B;
    DrawPixel(Video, X, Y, ColorU32);
}

internal_function void
DrawRectangle(platform_video *Video, rectangle *Rectangle, rgba *Color)
{
    for(int Y = Rectangle->Y; Y < (Rectangle->Y + Rectangle->Height); Y++)
    {
        for(int X = Rectangle->X; X < (Rectangle->X + Rectangle->Width); X++)
        {
            DrawPixelFromRGBA(Video, X, Y, Color);
        }
    }
}

internal_function void
DrawBresenhamLine(platform_video *Video, int X1, int Y1, int X2, int Y2, rgba *Color)
{
    int DeltaX = Absolute(X2 - X1);
    int DeltaY = -Absolute(Y2 - Y1);
    int StepX = (X1 < X2) ? (1) : (-1);
    int StepY = (Y1 < Y2) ? (1) : (-1);
    int Error = DeltaX + DeltaY;

    for(;;)
    {
        DrawPixelFromRGBA(Video, X1, Y1, Color);

        if((X1 == X2) && (Y1 == Y2))
        {
            break;
        }

        int Error2 = Error * 2;
        if(Error2 >= DeltaY)
        {
            Error += DeltaY;
            X1 += StepX;
        }
        
        if(Error2 <= DeltaX)
        {
            Error += DeltaX;
            Y1 += StepY;
        }
    }
}

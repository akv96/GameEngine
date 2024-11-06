#include "video.h"

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
DrawBresenhamLine(platform_video *Video, int X0, int Y0, int X1, int Y1, rgba *Color)
{
    int DeltaX = Absolute(X1 - X0);
    int StepX = (X0 < X1) ? (1) : (-1);
    int DeltaY = -Absolute(Y1 - Y0);
    int StepY = (Y0 < Y1) ? (1) : (-1);
    int Error = DeltaX + DeltaY;

    for(;;)
    {
        DrawPixelFromRGBA(Video, X0, Y0, Color);

        if((X0 == X1) && (Y0 == Y1))
        {
            break;
        }

        int Error2 = 2 * Error;
        if(Error2 >= DeltaY)
        {
            Error += DeltaY;
            X0 += StepX;
        }

        if(Error2 <= DeltaX)
        {
            Error += DeltaX;
            Y0 += StepY;
        }
    }
}

internal_function void
CreateMatrixTranslation(matrix4x4 *Matrix, f32 X, f32 Y, f32 Z)
{
    CreateMatrixIdentity(Matrix);

    Matrix->Data[0][3] = X;
    Matrix->Data[1][3] = Y;
    Matrix->Data[2][3] = Z;    
}

internal_function void
MatrixRotationX(matrix4x4 *Matrix, f32 Angle)
{
    CreateMatrixIdentity(Matrix);

    Matrix->Data[1][1] = Cosine(Angle);
    Matrix->Data[1][2] = -Sine(Angle);
    Matrix->Data[2][1] = Sine(Angle);
    Matrix->Data[2][2] = Cosine(Angle);
}

internal_function void
MatrixRotationY(matrix4x4 *Matrix, f32 Angle)
{
    CreateMatrixIdentity(Matrix);
    
    Matrix->Data[0][0] = Cosine(Angle);
    Matrix->Data[0][2] = Sine(Angle);
    Matrix->Data[2][0] = -Sine(Angle);
    Matrix->Data[2][2] = Cosine(Angle);
}

internal_function void
MatrixRotationZ(matrix4x4 *Matrix, f32 Angle)
{
    CreateMatrixIdentity(Matrix);
    
    Matrix->Data[0][0] = Cosine(Angle);
    Matrix->Data[0][1] = -Sine(Angle);
    Matrix->Data[1][0] = Sine(Angle);
    Matrix->Data[1][1] = Cosine(Angle);
}

internal_function void
MatrixPerspective(matrix4x4 *Matrix, f32 FieldOfView, f32 AspectRatio, f32 NearPlane, f32 FarPlane)
{
    f32 HalfFieldOfView = FieldOfView / 2.0f;

    Matrix->Data[0][0] = 1.0f / (AspectRatio * HalfFieldOfView);
    Matrix->Data[1][1] = 1.0f / HalfFieldOfView;
    Matrix->Data[2][2] = -(FarPlane + NearPlane) / (FarPlane - NearPlane);
    Matrix->Data[2][3] = -1.0f;
    Matrix->Data[3][2] = (-2.0f * FarPlane * NearPlane) / (FarPlane - NearPlane);
}

internal_function void
ProjectAndDrawTriangle(platform_video *Video, triangle *Triangle)
{
    v2 ScreenVertices[3] = {0};

    for(int Index = 0; Index < 3; Index++)
    {
        f32 W = Triangle->Vertices[Index].Z;
        ScreenVertices[Index].X = (Triangle->Vertices[Index].X / W) * ((f32)Video->Width / 2.0f) + ((f32)Video->Width / 2.0f);
        ScreenVertices[Index].Y = (Triangle->Vertices[Index].Y / W) * ((f32)Video->Height / 2.0f) + ((f32)Video->Height / 2.0f);
    }

    for(int Index = 0; Index < 3; Index++)
    {
        if((ScreenVertices[Index].X < 0) || (ScreenVertices[Index].X >= Video->Width) || (ScreenVertices[Index].Y < 0) || (ScreenVertices[Index].Y >= Video->Height))
        {
            return;
        }
    }

    int MinX = (int)Floor(ScreenVertices[0].X);
    int MaxX = (int)Floor(ScreenVertices[0].X);
    int MinY = (int)Floor(ScreenVertices[0].Y);
    int MaxY = (int)Floor(ScreenVertices[0].Y);

    for(int Index = 1; Index < 3; Index++)
    {
        MinX = (int)Min(MinX, Floor(ScreenVertices[Index].X));
        MaxX = (int)Max(MaxX, Floor(ScreenVertices[Index].X));
        MinY = (int)Min(MinY, Floor(ScreenVertices[Index].Y));
        MaxY = (int)Max(MaxY, Floor(ScreenVertices[Index].Y));
    }

    MinX = (int)Max(MinX, 0);
    MaxX = (int)Min(MaxX, Video->Width - 1);
    MinY = (int)Max(MinY, 0);
    MaxY = (int)Min(MaxY, Video->Height - 1);

    for(int Y = MinY; Y <= MaxY; Y++)
    {
        for(int X = MinX; X <= MaxX; X++)
        {
            f32 Alpha = ((ScreenVertices[1].Y - ScreenVertices[2].Y) * (X - ScreenVertices[2].X) + (ScreenVertices[2].X - ScreenVertices[1].X) * (Y - ScreenVertices[2].Y)) / ((ScreenVertices[1].Y - ScreenVertices[2].Y) * (ScreenVertices[0].X - ScreenVertices[2].X) + (ScreenVertices[2].X - ScreenVertices[1].X) * (ScreenVertices[0].Y - ScreenVertices[2].Y));                        
            f32 Beta = ((ScreenVertices[2].Y - ScreenVertices[0].Y) * (X - ScreenVertices[2].X) + (ScreenVertices[0].X - ScreenVertices[2].X) * (Y - ScreenVertices[2].Y)) / ((ScreenVertices[1].Y - ScreenVertices[2].Y) * (ScreenVertices[0].X - ScreenVertices[2].X) + (ScreenVertices[2].X - ScreenVertices[1].X) * (ScreenVertices[0].Y - ScreenVertices[2].Y));
            f32 Gamma = 1.0f - Alpha - Beta;

            if((Alpha >= 0.0f) && (Beta >= 0.0f) && (Gamma >= 0.0f))
            {
                f32 Z = Alpha * Triangle->Vertices[0].Z + Beta * Triangle->Vertices[1].Z + Gamma * Triangle->Vertices[2].Z;

                DrawPixel(Video, X, Y, 0xFFFFFFFF);
            }
        }
    }
}
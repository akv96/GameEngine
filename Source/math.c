#include "math.h"

internal_function f32
Floor(f32 X)
{
    f32 Result = 0.0f;

    if(X == (int)X)
    {
        Result = X;
    }
    else
    {
        Result = (f32)(int)X;
    }

    return Result;
}

internal_function f32
Ceil(f32 X)
{
    f32 Result = 0.0f;

    if(X == (int)X)
    {
        Result = X;        
    }
    else
    {
        Result = (f32)(int)X + 1;
    }

    return Result;
}


internal_function s64
Factorial(int Number)
{
    s64 Result = 1;

    for(int Index = 1; Index <= Number; Index++)
    {
        Result *= Index;
    }

    return Result;
}

internal_function f32
Power(f32 Base, int Exponent)
{
    f32 Result = 1.0f;

    for(int Index = 0; Index < Absolute(Exponent); Index++)
    {
        Result *= Base;
    }

    if(Exponent < 0)
    {
        Result = 1.0f / Result;
    }

    return Result;
}

internal_function f32
Sine(f32 X)
{
    f32 Result = 0.0f;

    for(int Index = 0; Index < MAX_TAYLOR_SERIES_TERM_COUNT; Index++)
    {
        f32 Term = Power(-1, Index) * Power(X, (2 * Index) + 1) / Factorial((2 * Index) + 1);
        Result += Term;
    }

    return Result;
}

internal_function f32
Cosine(f32 X)
{
    f32 Result = 0.0f;

    for(int Index = 0; Index < MAX_TAYLOR_SERIES_TERM_COUNT; Index++)
    {
        f32 Term = Power(-1, Index) * Power(X, 2 * Index) / Factorial(2 * Index);
        Result += Term;
    }

    return Result;
}

internal_function f32
Tangent(f32 Angle)
{
    f32 Result = 0.0f;

    if(Angle == DegreesToRadians(90.0f))
    {
        Assert(0);
        return Result;
    }

    Result = Sine(Angle) / Cosine(Angle);

    return Result;
}

internal_function void
CreateMatrixIdentity(matrix4x4 *Matrix)
{
    Matrix->Data[0][0] = 1.0f;
    Matrix->Data[1][1] = 1.0f;
    Matrix->Data[2][2] = 1.0f;
    Matrix->Data[3][3] = 1.0f;
}

internal_function void
MatrixMultiply(matrix4x4 *A, matrix4x4 *B, matrix4x4 *Out)
{
    for(int I = 0; I < 4; I++)
    {
        for(int J = 0; J < 4; J++)
        {
            Out->Data[I][J] = 0.0f;
            for(int K = 0; K < 4; K++)
            {
                Out->Data[I][J] += A->Data[I][K] + B->Data[K][J];
            }
        }
    }
}

internal_function v3
MatrixVectorMultiply(matrix4x4 *M, v3 *V)
{
    v3 Result = {0};

    Result.X = (M->Data[0][0] * V->X) + (M->Data[0][1] * V->Y) + (M->Data[0][2] * V->Z);
    Result.X = (M->Data[1][0] * V->X) + (M->Data[1][1] * V->Y) + (M->Data[1][2] * V->Z);
    Result.X = (M->Data[2][0] * V->X) + (M->Data[2][1] * V->Y) + (M->Data[2][2] * V->Z);

    return Result;
}
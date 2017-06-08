// Copyright 2017 Patrick Chadbourne (PChadbourne). All Rights Reserved.

#include "MarchingIslands.h"
#include "MarchingCubes.h"

MarchingCubes::MarchingCubes()
{
}

MarchingCubes::~MarchingCubes()
{
}

void MarchingCubes::Polyganize(FCell Cell, FMeshData* Data)
{
	int CubeIndex = 0;
	FVector VertList[12];

	if (Cell.val[0] < Isolevel) CubeIndex |= 1;
	if (Cell.val[1] < Isolevel) CubeIndex |= 2;
	if (Cell.val[2] < Isolevel) CubeIndex |= 4;
	if (Cell.val[3] < Isolevel) CubeIndex |= 8;
	if (Cell.val[4] < Isolevel) CubeIndex |= 16;
	if (Cell.val[5] < Isolevel) CubeIndex |= 32;
	if (Cell.val[6] < Isolevel) CubeIndex |= 64;
	if (Cell.val[7] < Isolevel) CubeIndex |= 128;

	if (edgeTable[CubeIndex] == 0) return;

	if (edgeTable[CubeIndex] & 1)
		VertList[0] = VertexInterp(Cell.p[0], Cell.p[1], Cell.val[0], Cell.val[1], 0);
	if (edgeTable[CubeIndex] & 2)
		VertList[1] = VertexInterp(Cell.p[1], Cell.p[2], Cell.val[1], Cell.val[2], 0);
	if (edgeTable[CubeIndex] & 4)
		VertList[2] = VertexInterp(Cell.p[2], Cell.p[3], Cell.val[2], Cell.val[3], 0);
	if (edgeTable[CubeIndex] & 8)
		VertList[3] = VertexInterp(Cell.p[3], Cell.p[0], Cell.val[3], Cell.val[0], 0);
	if (edgeTable[CubeIndex] & 16)
		VertList[4] = VertexInterp(Cell.p[4], Cell.p[5], Cell.val[4], Cell.val[5], 0);
	if (edgeTable[CubeIndex] & 32)
		VertList[5] = VertexInterp(Cell.p[5], Cell.p[6], Cell.val[5], Cell.val[6], 0);
	if (edgeTable[CubeIndex] & 64)
		VertList[6] = VertexInterp(Cell.p[6], Cell.p[7], Cell.val[6], Cell.val[7], 0);
	if (edgeTable[CubeIndex] & 128)
		VertList[7] = VertexInterp(Cell.p[7], Cell.p[4], Cell.val[7], Cell.val[4], 0);
	if (edgeTable[CubeIndex] & 256)
		VertList[8] = VertexInterp(Cell.p[0], Cell.p[4], Cell.val[0], Cell.val[4], 0);
	if (edgeTable[CubeIndex] & 512)
		VertList[9] = VertexInterp(Cell.p[1], Cell.p[5], Cell.val[1], Cell.val[5], 0);
	if (edgeTable[CubeIndex] & 1024)
		VertList[10] = VertexInterp(Cell.p[2], Cell.p[6], Cell.val[2], Cell.val[6], 0);
	if (edgeTable[CubeIndex] & 2048)
		VertList[11] = VertexInterp(Cell.p[3], Cell.p[7], Cell.val[3], Cell.val[7], 0);


	for (int i = 0; triTable[CubeIndex][i] != -1; i += 3) {


		Data->V.Add(VertList[triTable[CubeIndex][i]]);
		Data->T.Add(Data->T.Num());

		Data->V.Add(VertList[triTable[CubeIndex][i + 1]]);
		Data->T.Add(Data->T.Num());

		Data->V.Add(VertList[triTable[CubeIndex][i + 2]]);
		Data->T.Add(Data->T.Num());
	}
}


void MarchingCubes::ProcessGrid(TArray<float> Grid, int Width, FMeshData* Data, FVector A, FVector B, FVector C, FVector D) //Needs to reposition p values off of icosahedron
{
	int Width2 = Width * Width;
	FCell Cell;


	for (int i = 0; i < (Width - 1); i++)
	{
		for (int j = 0; j < (Width); j++)
		{
			for (int k = 0; k < (Width - 1); k++)
			{
				Cell.val[7] = Grid[i + (Width * j) + (Width2 * k)];
				Cell.p[7] = CalculateTriangularInterpolation(i, j, k, A, B, C, D, Width);

				Cell.val[4] = Grid[i + (Width * j) + (Width2 * k) + 1];
				Cell.p[4] = CalculateTriangularInterpolation(i+1, j, k, A, B, C, D, Width);

				Cell.val[6] = Grid[i + (Width * j) + (Width2 * k) + Width];
				Cell.p[6] = CalculateTriangularInterpolation(i, j+1, k, A, B, C, D, Width);

				Cell.val[5] = Grid[i + (Width * j) + (Width2 * k) + Width + 1];
				Cell.p[5] = CalculateTriangularInterpolation(i+1, j+1, k, A, B, C, D, Width);

				Cell.val[3] = Grid[i + (Width * j) + (Width2 * k) + Width2];
				Cell.p[3] = CalculateTriangularInterpolation(i, j, k+1, A, B, C, D, Width);

				Cell.val[0] = Grid[i + (Width * j) + (Width2 * k) + Width2 + 1];
				Cell.p[0] = CalculateTriangularInterpolation(i+1, j, k+1, A, B, C, D, Width);

				Cell.val[2] = Grid[i + (Width * j) + (Width2 * k) + Width2 + Width];
				Cell.p[2] = CalculateTriangularInterpolation(i, j+1, k+1, A, B, C, D, Width);

				Cell.val[1] = Grid[i + (Width * j) + (Width2 * k) + Width2 + Width + 1];
				Cell.p[1] = CalculateTriangularInterpolation(i+1, j+1, k+1, A, B, C, D, Width);

				Polyganize(Cell, Data);
			}
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("%d"), Triangles.Num());

	Data->N.SetNum(Data->V.Num(), false);

	for (int32 Index = 0; Index < Data->T.Num(); Index += 3)
	{
		const FVector Edge21 = Data->V[Data->T[Index + 1]] - Data->V[Data->T[Index + 2]];
		const FVector Edge20 = Data->V[Data->T[Index + 0]] - Data->V[Data->T[Index + 2]];
		FVector TriNormal = (Edge21 ^ Edge20).GetSafeNormal();

		TriNormal.Normalize();
		for (int32 i = 0; i < 3; i++)
		{
			Data->N[Data->T[Index + i]] = TriNormal;
		}
	}
}


FVector MarchingCubes::VertexInterp(FVector P1, FVector P2, float P1Val, float P2Val, float Value)
{


	float mu;
	FVector P;

	if (FMath::Abs(Value - P1Val) < 0.00001)
		return(P1);
	if (FMath::Abs(Value - P2Val) < 0.00001)
		return(P2);
	if (FMath::Abs(P1Val - P2Val) < 0.00001)
		return(P1);
	mu = (Value - P1Val) / (P2Val - P1Val);
	P.X = P1.X + mu * (P2.X - P1.X);
	P.Y = P1.Y + mu * (P2.Y - P1.Y);
	P.Z = P1.Z + mu * (P2.Z - P1.Z);

	return(P);
}

FVector MarchingCubes::CalculateTriangularInterpolation(int i, int j, int k, FVector A, FVector B, FVector C, FVector D, int Width)
{
	if (i + j < Width) {
		FVector alpha = FMath::Lerp(A, B, (i*2.0f) / (float)Width);
		FVector beta = FMath::Lerp(A, C, (j*2.0f) / (float)Width);
		return FMath::Lerp(alpha, beta, 0.5f) * 10 *(k / (float) Width);
	}
	else {
		FVector alpha = FMath::Lerp(D, C, (-i) / (float)Width+1);
		FVector beta = FMath::Lerp(D, B, (-j) / (float)Width+1);
		return (FMath::Lerp(alpha, beta, 0.5f)) * 10 * (k / (float) Width);
		//return FVector(0, 0, 0);
	}
}
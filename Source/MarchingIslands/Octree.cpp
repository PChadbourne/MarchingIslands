// Copyright 2017 Patrick Chadbourne (PChadbourne). All Rights Reserved.

#include "MarchingIslands.h"
#include "Octree.h"

Octree::Octree()
{
}

Octree::Octree(TArray<float> Grid, int Width)
{
	Corners[0] = FVector(0, 0, 0);
	Corners[1] = FVector(Width - 1, 0, 0);
	Corners[2] = FVector(0, Width - 1, 0);
	Corners[3] = FVector(Width - 1, Width - 1, 0);

	Corners[4] = FVector(0, 0, Width - 1);
	Corners[5] = FVector(Width - 1, 0, Width - 1);
	Corners[6] = FVector(0, Width - 1, Width - 1);
	Corners[7] = FVector(Width - 1, Width - 1, Width - 1);

	for (int i = 0; i < 8; i++)
		Values[i] = Grid[Corners[i].X + Corners[i].Y * Width + Corners[i].Z * Width * Width];

	Split();
}


Octree::~Octree()
{
}

void Octree::Split()
{
	//float InterpolatedValue = TrilinearInterpolation(Values);
	//float DensityValues[19] = GetDensityValues();
	//if (Error(InterpolatedValue, DensityValues) > SPLIT_VALUE) {

	//}
}

float TrilinearInterpolation(float Nodes[8])
{
	return (((((Nodes[0] + Nodes[1]) / 2) + ((Nodes[2] + Nodes[3]) / 2)) / 2) + ((((Nodes[4] + Nodes[5]) / 2) + ((Nodes[6] + Nodes[7]) / 2)) / 2)) / 2;
}

float Error(float InterpolatedValue, float DensityValues[19])
{
	float Sum = 0.0;
	for (int i = 0; i < 19; i++)
		Sum += InterpolatedValue - DensityValues[i];
	return FMath::Abs(Sum);
}

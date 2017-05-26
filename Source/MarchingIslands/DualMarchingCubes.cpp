// Copyright 2017 Patrick Chadbourne (PChadbourne). All Rights Reserved.

#include "MarchingIslands.h"
#include "DualMarchingCubes.h"

DualMarchingCubes::DualMarchingCubes()
{
	DualMarchingCubes(16, 16);
}

DualMarchingCubes::DualMarchingCubes(int resolution_, int size_)
{
	Resolution = resolution_;
	Size = size_;
}

DualMarchingCubes::~DualMarchingCubes()
{
}


void DualMarchingCubes::Contour(float threshold)
{
	for (int x = 0; x < Resolution; x++)
	{
		for (int y = 0; y < Resolution; y++)
		{
			for (int z = 0; z < Resolution; z++)
			{
				GenerateCells(x, y, z);
			}
		}
	}

	for (int x = 0; x < Resolution - 1; x++)
	{
		for (int y = 0; y < Resolution - 1; y++)
		{
			for (int z = 0; z < Resolution - 1; z++)
			{
				GenerateEdges(x, y, z);
			}
		}
	}

	//for (int x = Resolution - 2; x >= 0; x--)
	for (int x = 0; x < Resolution - 1; x++)
	{
		//for (int y = Resolution - 2; y >= 0; y-- )
		for (int y = 0; y < Resolution - 1; y++)
		{
			//for (int z = Resolution - 2; z >= 0; z--)
			for (int z = 0; z < Resolution - 1; z++)
			{
				Polygonize(x, y, z);
			}
		}
	}

	for (int x = 0; x < Resolution; x++)
	{
		for (int y = 0; y < Resolution; y++)
		{
			for (int z = 0; z < Resolution; z++)
			{
				GenerateIndices(x, y, z);
			}
		}
	}

	VertexCount = Vertices.Count;
	if (Indices != null)
		IndexCount = Indices.Count;

	if (Vertices.Count > 0)
		VertexBuffer.SetData<VertexPositionColorNormal>(0, Vertices.ToArray(), 0, VertexCount, VertexPositionColorNormal.VertexDeclaration.VertexStride);
	if (!UseFlatShading && Indices.Count > 0)
		IndexBuffer.SetData<int>(Indices.ToArray());
}

FVector Edge::GetIntersection()
{
	return A + (-ValueA) * (B - A) / (ValueB - ValueA);;
}

Cell::Cell()
{
	for (int i = 0; i < 12; i++)
	{
		Edges[i].Index = i;
	}
}
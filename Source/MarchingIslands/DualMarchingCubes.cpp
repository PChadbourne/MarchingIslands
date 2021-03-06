// Copyright 2017 Patrick Chadbourne (PChadbourne). All Rights Reserved.

#include "MarchingIslands.h"
#include "DualMarchingCubes.h"
#include "SimplexNoise.h"
/*
DualMarchingCubes::DualMarchingCubes()
{
	DualMarchingCubes(16, 16);
}

DualMarchingCubes::DualMarchingCubes(int resolution_, int size_)
{
	Resolution = resolution_;
	Size = size_;

	Grid.Init(0.0f, (Resolution + 2) * (Resolution + 2) * (Resolution + 2));
	SimplexNoise Noise;

	float scale = .05;

	for (int i = 1; i < Resolution - 1; i++) {
		for (int j = 1; j < Resolution - 1; j++) {
			for (int k = 1; k < Resolution - 1; k++) {
				Grid[i + Resolution*j + Resolution*Resolution*k] = Noise.SimplexNoise3D(i*scale, j*scale, k*scale);
			}
		}
	}
}

DualMarchingCubes::~DualMarchingCubes()
{
}


void DualMarchingCubes::Contour(float Threshold)
{
	Cells.Reserve(Resolution*Resolution*Resolution);
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
	/*
	VertexCount = Vertices.Count;
	if (Indices != null)
		IndexCount = Indices.Count;

	if (Vertices.Count > 0)
		VertexBuffer.SetData<VertexPositionColorNormal>(0, Vertices.ToArray(), 0, VertexCount, VertexPositionColorNormal.VertexDeclaration.VertexStride);
	if (!UseFlatShading && Indices.Count > 0)
		IndexBuffer.SetData<int>(Indices.ToArray());
		
}

void DualMarchingCubes::GenerateCells(int x, int y, int z)
{
	Cells[x + (y * Resolution) + (z * Resolution * Resolution)].Edges[0] = GenerateEdge(x, y, z, 0);
	Cells[x + (y * Resolution) + (z * Resolution * Resolution)].Edges[3] = GenerateEdge(x, y, z, 3);
	Cells[x + (y * Resolution) + (z * Resolution * Resolution)].Edges[8] = GenerateEdge(x, y, z, 8);
}

Edge DualMarchingCubes::GenerateEdge(int x, int y, int z, int i)
{
	Edge E = Edge();
	E.Index = i;
	E.A = FVector(x, y, z) + CornerDeltas[EdgePairs[i][0]];
	E.B = FVector(x, y, z) + CornerDeltas[EdgePairs[i][1]];

	E.ValueA = Grid[E.A.X + E.A.Y * (Resolution + 2) + E.A.Z * (Resolution + 2) * (Resolution + 2)];
	E.ValueB = Grid[E.B.X + E.B.Y * (Resolution + 2) + E.B.Z * (Resolution + 2) * (Resolution + 2)];

	return E;
}

void DualMarchingCubes::GenerateEdges(int x, int y, int z)
{
	Cells[x + (y * Resolution) + (z * Resolution * Resolution)].Edges[1] = Cells[x + 1 + y * Resolution + z * Resolution * Resolution].Edges[3];
	Cells[x + (y * Resolution) + (z * Resolution * Resolution)].Edges[2] = Cells[x + y * Resolution + (z + 1) * Resolution * Resolution].Edges[0];

	Cells[x + (y * Resolution) + (z * Resolution * Resolution)].Edges[4] = Cells[x + (y + 1) * Resolution + z * Resolution * Resolution].Edges[0];
	Cells[x + (y * Resolution) + (z * Resolution * Resolution)].Edges[5] = Cells[x + 1 + (y + 1) * Resolution + z * Resolution * Resolution].Edges[3];
	Cells[x + (y * Resolution) + (z * Resolution * Resolution)].Edges[6] = Cells[x + (y + 1) * Resolution + (z + 1) * Resolution * Resolution].Edges[0];
	Cells[x + (y * Resolution) + (z * Resolution * Resolution)].Edges[7] = Cells[x + (y + 1) * Resolution + z * Resolution * Resolution].Edges[3];

	Cells[x + (y * Resolution) + (z * Resolution * Resolution)].Edges[9] = Cells[x + 1 + y * Resolution + z * Resolution * Resolution].Edges[8];
	Cells[x + (y * Resolution) + (z * Resolution * Resolution)].Edges[10] = Cells[x + 1 + y * Resolution + (z + 1) * Resolution * Resolution].Edges[8];
	Cells[x + (y * Resolution) + (z * Resolution * Resolution)].Edges[11] = Cells[x + y * Resolution + (z + 1) * Resolution * Resolution].Edges[8];
}

void DualMarchingCubes::Polygonize(int x, int y, int z)
{
	if (Cells[x + (y * Resolution) + (z * Resolution * Resolution)] == nullptr)
		return;

	int cube_index = 0;
	for (int i = 0; i < 8; i++)
	{
		if (Grid[x + CornerDeltas[i].X + (y + CornerDeltas[i].Y) * Resolution + (z + CornerDeltas[i].Z) * Resolution * Resolution] < 0)
			cube_index |= 1 << i;
	}

	if (cube_index == 0 || cube_index == 255)
		return;

	//Cells[x + (y * Resolution) + (z * Resolution * Resolution)].Vertices = new Vertex[VerticesNumberTable[cube_index]];

	int v_index = 0;
	Cells[x + (y * Resolution) + (z * Resolution * Resolution)].Vertices[0] = Vertex();
	for (int e = 0; e < 16; e++)
	{
		if (EdgesTable[cube_index][e] == -2)
			break;
		if (EdgesTable[cube_index][e] == -1)
		{
			v_index++;
			continue;
		}

		Cells[x + (y * Resolution) + (z * Resolution * Resolution)].Vertices[v_index].Edges.Add(EdgesTable[cube_index][e]);
		Cells[x + (y * Resolution) + (z * Resolution * Resolution)].Edges[EdgesTable[cube_index][e]].Vertices.Add(Cells[x + (y * Resolution) + (z * Resolution * Resolution)].Vertices[v_index]);
	}

	foreach(Vertex v in Cells[x + (y * Resolution) + (z * Resolution * Resolution)].Vertices)
	{
		Vertex tx = v;
		if (v == null) //for edges 241/243, which were originally marked as having 2 vertices...?
			continue;
		Vector3 point = new Vector3();

		if (VertexMode != VertexModes.Block)
		{
			//QEF3D qef = new QEF3D();
			QEFProper.QEFSolver qef = new QEFProper.QEFSolver();
			VertexPlacement qem = new VertexPlacement();
			for (int e_i = 0; e_i < tx.Edges.Count; e_i++)
			{
				Edge e = Cells[x + (y * Resolution) + (z * Resolution * Resolution)].Edges[tx.Edges[e_i]];
				if (VertexMode == VertexModes.Edges)
					point += e.GetIntersection();
				else if (VertexMode == VertexModes.QEF)
					qef.Add(e.GetIntersection() - new Vector3(x, y, z), Sampler.GetNormal(e.GetIntersection()));
				else
					qem.AddPlane(e.GetIntersection() - new Vector3(x, y, z), Sampler.GetNormal(e.GetIntersection()));
			}

			if (VertexMode == VertexModes.Edges)
				point /= (float)tx.Edges.Count;
			else if (VertexMode == VertexModes.QEF)
				point = qef.Solve(1e-6f, 4, 1e-6f) + new Vector3(x, y, z);
			else
				point = qem.Solve() + new Vector3(x, y, z);
		}
		else
			point = new Vector3(x, y, z) + Vector3.One * 0.5f;
		//point = Vector3.Clamp(point, new Vector3(x, y, z), new Vector3(x + 1, y + 1, z + 1));

		tx.Position = point;
		Vector3 norm = Sampler.GetNormal(point);
		Vector3 c_v = norm * 0.5f + Vector3.One * 0.5f;
		c_v.Normalize();
		Color clr = new Color(c_v);
		if (!UseFlatShading)
		{
			tx.Index = Vertices.Count;
			FVector pv = FVector(tx.Position, clr, norm);
			Vertices.Add(pv);
		}
		else
		{
			tx.Index = CalculatedVertices.Count;
			VertexPositionColor pv = FVector(tx.Position, clr);
			CalculatedVertices.Add(pv);
		}

		VertexPositionColor[] vs = FVector[24];
		Color c = Color.Red;
		float vx = tx.Position.X;
		float vy = tx.Position.Y;
		float vz = tx.Position.Z;
		float r = 0.25f;
		vs[0] = FVector((vx + 0), (vy + 0), (vz + 0));
		vs[1] = FVector((vx + r), (vy + 0), (vz + 0));
		vs[2] = FVector((vx + r), (vy + 0), (vz + 0));
		vs[3] = FVector((vx + r), (vy + r), (vz + 0));
		vs[4] = FVector((vx + r), (vy + r), (vz + 0));
		vs[5] = FVector((vx + 0), (vy + r), (vz + 0));
		vs[6] = FVector((vx + 0), (vy + r), (vz + 0));
		vs[7] = FVector((vx + 0), (vy + 0), (vz + 0));

		vs[8] = FVector((vx + 0), (vy + 0), (vz + r));
		vs[9] = FVector((vx + r), (vy + 0), (vz + r));
		vs[10] = FVector((vx + r), (vy + 0), (vz + r));
		vs[11] = FVector((vx + r), (vy + r), (vz + r));
		vs[12] = FVector((vx + r), (vy + r), (vz + r));
		vs[13] = FVector((vx + 0), (vy + r), (vz + r));
		vs[14] = FVector((vx + 0), (vy + r), (vz + r));
		vs[15] = FVector((vx + 0), (vy + 0), (vz + r));

		vs[16] = FVector((vx + 0), (vy + 0), (vz + 0));
		vs[17] = FVector((vx + 0), (vy + 0), (vz + r));
		vs[18] = FVector((vx + 0), (vy + r), (vz + 0));
		vs[19] = FVector((vx + 0), (vy + r), (vz + r));

		vs[20] = FVector((vx + r), (vy + 0), (vz + 0));
		vs[21] = FVector((vx + r), (vy + 0), (vz + r));
		vs[22] = FVector((vx + r), (vy + r), (vz + 0));
		vs[23] = FVector((vx + r), (vy + r), (vz + r));

		OutlineBuffer.SetData<VertexPositionColor>(OutlineLocation * VertexPositionColor.VertexDeclaration.VertexStride, vs, 0, 24, VertexPositionColor.VertexDeclaration.VertexStride);
		OutlineLocation += 24;
	}
}

void DualMarchingCubes::GenerateIndices(int x, int y, int z)
{
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
*/
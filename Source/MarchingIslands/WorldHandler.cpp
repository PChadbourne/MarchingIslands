// Copyright 2017 Patrick Chadbourne (PChadbourne). All Rights Reserved.

#include "MarchingIslands.h"
#include "WorldHandler.h"
#include "RuntimeMeshComponent.h"
#include "MarchingCubes.h"

// Sets default values
AWorldHandler::AWorldHandler()
{
	RMC = CreateDefaultSubobject<URuntimeMeshComponent>(TEXT("RuntimeMesh"));
	RootComponent = RMC;

	float t = (1.0 + FMath::Sqrt(5)) / 2.0;

	//Locations of the vertices of the Icosahedron
	IcoVerts.Add(FVector(-100, t * 100, 0));
	IcoVerts.Add(FVector(100, t * 100, 0));
	IcoVerts.Add(FVector(-100, -t * 100, 0));
	IcoVerts.Add(FVector(100, -t * 100, 0));

	IcoVerts.Add(FVector(0, -100, t * 100));
	IcoVerts.Add(FVector(0, 100, t * 100));
	IcoVerts.Add(FVector(0, -100, -t * 100));
	IcoVerts.Add(FVector(0, 100, -t * 100));

	IcoVerts.Add(FVector(t * 100, 0, -100));
	IcoVerts.Add(FVector(t * 100, 0, 100));
	IcoVerts.Add(FVector(-t * 100, 0, -100));
	IcoVerts.Add(FVector(-t * 100, 0, 100));


	//Vectors holding the indices of the triangle vertices
	//Each vector represents one of the 20 faces of the Icosahedron
	IcoTris.Add(FVector(0, 11, 5));
	IcoTris.Add(FVector(5, 11, 4));

	IcoTris.Add(FVector(0, 5, 1));
	IcoTris.Add(FVector(1, 5, 9));

	IcoTris.Add(FVector(0, 1, 7));
	IcoTris.Add(FVector(7, 1, 8));

	IcoTris.Add(FVector(0, 7, 10));
	IcoTris.Add(FVector(10, 7, 6));

	IcoTris.Add(FVector(0, 10, 11));
	IcoTris.Add(FVector(11, 10, 2));

	IcoTris.Add(FVector(3, 9, 4));
	IcoTris.Add(FVector(4, 9, 5));

	IcoTris.Add(FVector(3, 8, 9));
	IcoTris.Add(FVector(9, 8, 1));

	IcoTris.Add(FVector(3, 6, 8));
	IcoTris.Add(FVector(8, 6, 7));
	
	IcoTris.Add(FVector(3, 2, 6));
	IcoTris.Add(FVector(6, 2, 10));
	
	IcoTris.Add(FVector(3, 4, 2));
	IcoTris.Add(FVector(2, 4, 11));
	
	for (int i = 0; i < 20; i += 2)
	{
		//Pair two triangles so that their data can be stored in a square array.
		IcoSections.Add(FIcoSection(IcoTris[i], IcoTris[i+1]));
		IcoSections[i / 2].Heightmap = GenerateHeightmap(IcoSections[i / 2]);
	}

	GenerateMesh();
	RootComponent->UpdateBounds();
}

// Called when the game starts or when spawned
void AWorldHandler::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWorldHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<int> AWorldHandler::GetNeighboringSections(int CurrentTri)
{
	TArray<int> ReturnArray;
	if (CurrentTri < 5)
	{
		ReturnArray.Add((CurrentTri - 1) % 5);
		ReturnArray.Add((CurrentTri + 1) % 5);
		ReturnArray.Add(CurrentTri + 5);
		return ReturnArray;
	}
	if (CurrentTri < 10)
	{
		ReturnArray.Add(10 + (CurrentTri - 1) % 5);
		ReturnArray.Add(CurrentTri + 5);
		ReturnArray.Add(CurrentTri - 5);
		return ReturnArray;
	}
	if (CurrentTri < 15)
	{
		ReturnArray.Add(5 + (CurrentTri + 1) % 5);
		ReturnArray.Add(CurrentTri + 5);
		ReturnArray.Add(CurrentTri - 5);
		return ReturnArray;
	}
	if (CurrentTri < 15)
	{
		ReturnArray.Add(15 + (CurrentTri + 1) % 5);
		ReturnArray.Add(15 + (CurrentTri - 1) % 5);
		ReturnArray.Add(CurrentTri - 5);
		return ReturnArray;
	}
	return ReturnArray;
}

TArray<float> AWorldHandler::GenerateHeightmap(FIcoSection Section)
{
	TArray<float> ReturnArray;
	//One side of the array has to be larger so that the two triangles don't share the diagonal
	ReturnArray.Init(0.0f, Resolution*(Resolution + 1)/2);
	ReturnArray.Init(0.0f, Resolution*(Resolution + 1));
	return ReturnArray;
	for (int i = 0; i < Resolution; i++)
	{
		for (int j = 0; j < Resolution + 1; j++)
		{
			if (i + j < Resolution) {
				FVector alpha = FMath::Lerp(IcoVerts[Section.IndexA.X], IcoVerts[Section.IndexA.Y], (i*2.0f) / (float)Resolution);
				FVector beta = FMath::Lerp(IcoVerts[Section.IndexA.X], IcoVerts[Section.IndexA.Z], (j*2.0f) / (float)Resolution);
				FVector total = FMath::Lerp(alpha, beta, 0.5f);
				ReturnArray[i + Resolution * j] = Noise.SimplexNoise3D(total.X, total.Y, total.Z);
			}
			else {
				FVector alpha = FMath::Lerp(IcoVerts[Section.IndexB.Z], IcoVerts[Section.IndexB.Y], (-i) / (float)Resolution + 1);
				FVector beta = FMath::Lerp(IcoVerts[Section.IndexB.Z], IcoVerts[Section.IndexB.X], (-j) / (float)Resolution + 1);
				FVector total = (FMath::Lerp(alpha, beta, 0.5f));
				ReturnArray[i + Resolution * j] = Noise.SimplexNoise3D(total.X, total.Y, total.Z);
			}
		}
	}
	return ReturnArray;
}

TArray<float> AWorldHandler::HeightmapToVolume(TArray<float> Heightmap)
{
	TArray<float> ReturnArray;
	ReturnArray.Init(0.0f, Resolution*(Resolution+1)*(Resolution));

	for (int i = 0; i < Resolution; i++) {
		for (int j = 0; j < Resolution; j++) {
			for (int k = 0; k < Resolution; k++) {
				//ReturnArray[i + j*Resolution + k*Resolution*Resolution] = Noise.SimplexNoise3D(i*.05,j*.05,k*.05);
				//ReturnArray[i + j*Resolution + k*Resolution*Resolution] = Heightmap[i + j*Resolution];

				if (k > Resolution / 2) {
					ReturnArray[i + j*Resolution + k*Resolution*Resolution] = -1.0f;
				}
				else
				{
					ReturnArray[i + j*Resolution + k*Resolution*Resolution] = 1.0f;
				}
			}
		}
	}
	return ReturnArray;
}

void AWorldHandler::GenerateMesh()
{
	MarchingCubes MC = MarchingCubes();
	for (int i = 0; i < IcoSections.Num(); i++)
	{
		FMeshData Data;
		MC.ProcessGrid(HeightmapToVolume(IcoSections[i].Heightmap), Resolution, &Data, IcoVerts[IcoSections[i].IndexA.X], IcoVerts[IcoSections[i].IndexA.Y], IcoVerts[IcoSections[i].IndexA.Z], IcoVerts[IcoSections[i].IndexB.Z]);

		TArray<FVector2D> TexCoords;
		TArray<FColor> VertColors;
		TArray<FRuntimeMeshTangent> Tangents;
		
		TexCoords.SetNum(Data.V.Num(), false);
		VertColors.SetNum(Data.V.Num(), false);
		Tangents.SetNum(Data.V.Num(), false);

		RMC->CreateMeshSection(i, Data.V, Data.T, Data.N, TexCoords, VertColors, Tangents, true);
	}
	
}

FIcoSection::FIcoSection()
{
}

FIcoSection::FIcoSection(FVector A, FVector B)
{
	IndexA = A;
	IndexB = B;

}
// Copyright 2017 Patrick Chadbourne (PChadbourne). All Rights Reserved.

#include "MarchingIslands.h"
#include "WorldHandler.h"

// Sets default values
AWorldHandler::AWorldHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	float t = (1.0 + FMath::Sqrt(5)) / 2.0;

	IcoVerts.Add(FVector(-1, t, 0));
	IcoVerts.Add(FVector(1, t, 0));
	IcoVerts.Add(FVector(-1, -t, 0));
	IcoVerts.Add(FVector(1, -t, 0));

	IcoVerts.Add(FVector(0, -1, t));
	IcoVerts.Add(FVector(0, 1, t));
	IcoVerts.Add(FVector(0, -1, -t));
	IcoVerts.Add(FVector(0, 1, -t));

	IcoVerts.Add(FVector(t, 0, -1));
	IcoVerts.Add(FVector(t, 0, 1));
	IcoVerts.Add(FVector(-t, 0, -1));
	IcoVerts.Add(FVector(-t, 0, 1));

	IcoTris.Add(FVector(0, 11, 5));
	IcoTris.Add(FVector(0, 5, 1));
	IcoTris.Add(FVector(0, 1, 7));
	IcoTris.Add(FVector(0, 7, 10));
	IcoTris.Add(FVector(0, 10, 11));

	IcoTris.Add(FVector(1, 5, 9));
	IcoTris.Add(FVector(5, 11, 4));
	IcoTris.Add(FVector(11, 10, 2));
	IcoTris.Add(FVector(10, 7, 6));
	IcoTris.Add(FVector(7, 1, 8));

	IcoTris.Add(FVector(3, 9, 4));
	IcoTris.Add(FVector(3, 4, 2));
	IcoTris.Add(FVector(3, 2, 6));
	IcoTris.Add(FVector(3, 6, 8));
	IcoTris.Add(FVector(3, 8, 9));

	IcoTris.Add(FVector(4, 9, 5));
	IcoTris.Add(FVector(2, 4, 11));
	IcoTris.Add(FVector(6, 2, 10));
	IcoTris.Add(FVector(8, 6, 7));
	IcoTris.Add(FVector(9, 8, 1));

	for (int i = 0; i < 20; i += 2)
	{
		IcoSections.Add(FIcoSection(IcoTris[i], IcoTris[i+1]));
		IcoSections[i / 2].Heightmap = GenerateHeightmap(IcoSections[i / 2]);
	}

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
	ReturnArray.Init(0.0f, Resolution*Resolution);
	for (int i = 0; i < Resolution; i++)
	{
		for (int j = 0; j < Resolution; j++)
		{
			if (i + j < Resolution) {
				FVector A = FMath::Lerp(IcoVerts[Section.IndexA.X], IcoVerts[Section.IndexA.Y], (float)i / (float)Resolution);
				FVector B = FMath::Lerp(IcoVerts[Section.IndexA.X], IcoVerts[Section.IndexA.Z], (float)j / (float)Resolution);
				FVector C = A + B;
				ReturnArray[i + Resolution * j] = Noise.SimplexNoise3D(C.X, C.Y, C.Z);
			} //Everything in here is probably wrong, will need to entirely re-do math to get proper values
			else {
				FVector A = FMath::Lerp(IcoVerts[Section.IndexB.X], IcoVerts[Section.IndexB.Y], (float)i / (float)Resolution);
				FVector B = FMath::Lerp(IcoVerts[Section.IndexB.X], IcoVerts[Section.IndexB.Z], (float)j / (float)Resolution);
				FVector C = A + B;
				ReturnArray[i + Resolution * j] = Noise.SimplexNoise3D(C.X, C.Y, C.Z);
			}
		}
	}
	return ReturnArray;
}

FIcoSection::FIcoSection()
{
}

FIcoSection::FIcoSection(FVector A, FVector B)
{
	IndexA = A;
	IndexB = B;
}
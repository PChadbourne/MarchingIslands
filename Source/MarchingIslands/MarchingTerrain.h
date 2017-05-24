// Copyright 2017 Patrick Chadbourne (PChadbourne). All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "MarchingTerrain.generated.h"


USTRUCT()
struct FTriangle
{
	GENERATED_BODY()

public:

	UPROPERTY()
		FVector p[3];
};


USTRUCT()
struct FGridcell
{
	GENERATED_BODY()

public:

	UPROPERTY()
		FVector p[8];

	UPROPERTY()
		float val[8];
};

UCLASS()
class MARCHINGISLANDS_API AMarchingTerrain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMarchingTerrain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Takes a singular grid cell and produces the triangles needed to render it.
	UFUNCTION()
	void MarchingCubes(FGridcell Gridcell, float Isolevel);

	//Update the entire mesh, iterating through all grid cells
	UFUNCTION(BlueprintCallable)
	void UpdateMesh(float Isolevel);

	//Interpolate between Vertexes with their values
	UFUNCTION()
	FVector VertexInterp(FVector P1, FVector P2);

public:

	//Width of the grid
	UPROPERTY()
	int Width = 17;

	//Runtime Mesh Component
	UPROPERTY(VisibleAnywhere)
	class URuntimeMeshComponent * RMC;

	//Array of scalar gridpoints to build surface on
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<float> Grid;


private:

	//Array of vector vertices on our isosurface
	//UPROPERTY()
	//TArray<FVector> Vertices;

	//Array of triangle indices of our isosurface
	//UPROPERTY()
	//TArray<int32> Triangles;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RuntimeMeshComponent.h"
#include "MarchingRuntimeMeshComponent.generated.h"


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
class MARCHINGISLANDS_API UMarchingRuntimeMeshComponent : public URuntimeMeshComponent
{
	GENERATED_BODY()

	//Takes a singular grid cell and produces the triangles needed to render it.
	UFUNCTION(BlueprintCallable)
	void MarchingCubes(FGridcell Grid, float Isolevel);
	
private:
	
	//Array of scalar gridpoints to build surface on
	UPROPERTY()
	TArray<float> Grid;
	
	//Array of vector vertices on our isosurface
	UPROPERTY()
	TArray<FVector> Vertices;

	//Array of triangle indices of our isosurface
	UPROPERTY()
	TArray<int32> Triangles;

};

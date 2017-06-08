// Copyright 2017 Patrick Chadbourne (PChadbourne). All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "SimplexNoise.h"
#include "MarchingCubes.h"
#include "WorldHandler.generated.h"

USTRUCT()
struct FIcoSection
{
	GENERATED_BODY()

public:
	FIcoSection();

	FIcoSection(FVector A, FVector B);

	UPROPERTY()
	TArray<float> Heightmap;

	FVector IndexA;
	FVector IndexB;
};

UCLASS()
class MARCHINGISLANDS_API AWorldHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldHandler();

	UPROPERTY(VisibleAnywhere)
	class URuntimeMeshComponent * RMC;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//
	TArray<FMeshData> MeshData;

	//
	UPROPERTY()
	TArray<FVector> IcoVerts;

	//
	UPROPERTY()
	TArray<FVector> IcoTris;

	//
	UPROPERTY()
	TArray<FIcoSection> IcoSections;

	static const int Resolution = 16;

	static SimplexNoise Noise;
	
	//
	UFUNCTION()
	TArray<int> GetNeighboringSections(int CurrentTri);

	//
	UFUNCTION()
	TArray<float> GenerateHeightmap(FIcoSection Section);

	//
	UFUNCTION()
	TArray<float> HeightmapToVolume(TArray<float> Heightmap);

	UFUNCTION(BlueprintCallable)
	void GenerateMesh();
};

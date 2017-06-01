// Copyright 2017 Patrick Chadbourne (PChadbourne). All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "WorldHandler.generated.h"


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ECardinalDirection : uint8
{
	CD_North 	UMETA(DisplayName = "North"),
	CD_East 	UMETA(DisplayName = "East"),
	CD_South	UMETA(DisplayName = "South"),
	CD_West		UMETA(DisplayName = "West")
};



USTRUCT()
struct FIcosphereSection
{
	GENERATED_BODY()

public:
	TArray<float> Height;
	int GetNeighboringSection(int SectionIndex, ECardinalDirection Direction);
};


UCLASS()
class MARCHINGISLANDS_API AWorldHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldHandler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Array of icosphere's sections. Each section is a rectangular representation of two triangles.
	UPROPERTY()
	TArray<FIcosphereSection> Sections;
	
};

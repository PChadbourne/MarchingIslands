// Copyright 2017 Patrick Chadbourne (PChadbourne). All Rights Reserved.

#include "MarchingIslands.h"
#include "WorldHandler.h"


// Sets default values
AWorldHandler::AWorldHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

//Handles the finding of neighbors on an icosphere given the section number and a direction.
int FIcosphereSection::GetNeighboringSection(int SectionIndex, ECardinalDirection Direction)
{
	switch (SectionIndex / 5)
	{
	case 0: //First 5 Sections
		switch (Direction)
		{
		case ECardinalDirection::CD_North: return (SectionIndex + 4) % 5;
		case ECardinalDirection::CD_East: return (SectionIndex + 1) % 5;
		case ECardinalDirection::CD_South: return (SectionIndex * 2) + 5;
		case ECardinalDirection::CD_West: return (SectionIndex * 2) + 4;
		}
		break;
	case 1: //Second 5 Sections
	case 2: //Third 5 Sections
		switch (Direction)
		{
		case ECardinalDirection::CD_North: return (SectionIndex % 2)*(5 + (SectionIndex + 4) % 10) + ((SectionIndex + 1) % 2)*(-10);
		case ECardinalDirection::CD_East: return (SectionIndex % 2)*((SectionIndex - 5) / 2) + ((SectionIndex + 1) % 2)*(5 + (SectionIndex - 4) % 10);
		case ECardinalDirection::CD_South: return SectionIndex + 10;
		case ECardinalDirection::CD_West: return 15 + (SectionIndex + 4) % 10;
		}
		break;
	case 3: //Fourth 5
	case 4: //Fifth 5
		switch (Direction)
		{
		case ECardinalDirection::CD_North: return SectionIndex - 10;
		case ECardinalDirection::CD_East: return 5 + (SectionIndex - 4) % 10;
		case ECardinalDirection::CD_South: return SectionIndex + 10;
		case ECardinalDirection::CD_West: return 25 + (SectionIndex + 4) % 10;
		}
		break;
	case 5: //Sixth 5 Sections
	case 6: //Seventh 5 Sections
		switch (Direction)
		{
		case ECardinalDirection::CD_North: return SectionIndex - 10;
		case ECardinalDirection::CD_East: return 15 + (SectionIndex - 4) % 10;
		case ECardinalDirection::CD_South: return (SectionIndex % 2)*(22 + (SectionIndex + 1) / 2) + ((SectionIndex + 1) % 2)*(25 + (SectionIndex - 4) % 10);
		case ECardinalDirection::CD_West: return (SectionIndex % 2)*(25 + (SectionIndex + 4) % 10) + ((SectionIndex + 1) % 2)*(22 + (SectionIndex / 2));
		}
		break;
	case 7: //Final 5 Sections
		switch (Direction)
		{
		case ECardinalDirection::CD_North: return (SectionIndex * 2) - 45;
		case ECardinalDirection::CD_East: return (SectionIndex * 2) - 44;
		case ECardinalDirection::CD_South: return 35 + (SectionIndex + 1) % 5;
		case ECardinalDirection::CD_West: return 35 + (SectionIndex + 4) % 5;
		}
		break;
	default: return -1;
	}
	return -1;
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "AIProject.h"
#include "NavigationVolume.h"
#include "WaypointComponent.h"

// Sets default values
ANavigationVolume::ANavigationVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxVolume = CreateDefaultSubobject<UBoxComponent>(FName("Volume"));
	RootComponent = BoxVolume;

	recursionIndex = 0;
}

// Called when the game starts or when spawned
void ANavigationVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANavigationVolume::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

#if WITH_EDITOR
void ANavigationVolume::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	WaypointList.Empty();
	DivideVolume(BoxVolume, DivideX, DivideY, DivideZ, WaypointList);

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

void ANavigationVolume::DivideVolume(UBoxComponent* volume, int32 divX, int32 divY, int32 divZ, TArray<UWaypointComponent*>& waypoints)
{
	const auto maxPosition = volume->GetUnscaledBoxExtent() * 1.0f;
	const auto minPosition = volume->GetUnscaledBoxExtent() * -1.0f;
	const auto divideScale = FVector(static_cast<float>(divX), static_cast<float>(divY), static_cast<float>(divZ));
	const auto deltaVector = (maxPosition - minPosition) / divideScale;
	const auto initPosition = minPosition + deltaVector * 0.5f;
	auto position = initPosition;

	for (auto x = initPosition.X; x < maxPosition.X; x += deltaVector.X)
	{
		for (auto y = initPosition.Y; y < maxPosition.Y; y += deltaVector.Y)
		{
			for (auto z = initPosition.Z; z < maxPosition.Z; z += deltaVector.Z)
			{
				waypoints.Add(CreateWaypoint(position, deltaVector * 0.5f, volume));
			}
		}
	}
}

UWaypointComponent* ANavigationVolume::CreateWaypoint(FVector location, FVector extent, USceneComponent* inParent)
{
	auto* waypoint = NewObject<UWaypointComponent>(this, FName("Waypoint"));
	waypoint->SetRelativeLocation(location);
	waypoint->ComponentTags.Add(TEXT("Waypoint"));
	if (inParent != nullptr)
	{
		waypoint->AttachTo(inParent);
	}
	auto* waypointCollision = NewObject<UBoxComponent>(this, FName("WaypointCollision"));
	waypointCollision->ComponentTags.Add(FName("Waypoint"));
	waypointCollision->SetBoxExtent(extent);
	if (inParent != nullptr)
	{
		waypointCollision->AttachTo(waypoint);
	}
	
	return waypoint;
}

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
void  ANavigationVolume::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	WaypointList.Empty();
	DestroyChildrenComponents(BoxVolume);
	DivideVolume(BoxVolume, DivideX, DivideY, DivideZ);
	auto waypoints = GetComponentsByClass(UWaypointComponent::StaticClass());
	for (auto waypoint : waypoints)
	{
		CreateOctree(Cast<UWaypointComponent>(waypoint), Recursion, recursionIndex);
	}
}
#endif

void ANavigationVolume::DivideVolume(UBoxComponent* volume, int32 divX, int32 divY, int32 divZ)
{
	const auto maxPosition = volume->GetUnscaledBoxExtent() * 1.0f;
	const auto minPosition = volume->GetUnscaledBoxExtent() * -1.0f;
	const auto divideScale = FVector(static_cast<float>(divX), static_cast<float>(divY), static_cast<float>(divZ));
	const auto deltaVector = (maxPosition - minPosition) / divideScale;
	const auto initPosition = minPosition + deltaVector * 0.5f;
	auto num = 0;

	for (auto x = initPosition.X; x < maxPosition.X; x += deltaVector.X)
	{
		for (auto y = initPosition.Y; y < maxPosition.Y; y += deltaVector.Y)
		{
			for (auto z = initPosition.Z; z < maxPosition.Z; z += deltaVector.Z)
			{
				CreateWaypoint(FVector(x, y, z), deltaVector * 0.5f, volume, num++);
			}
		}
	}
}

UWaypointComponent* ANavigationVolume::CreateWaypoint(FVector location, FVector extent, USceneComponent* inParent, int32 number)
{
	auto waypointName = "Waypoint" + FString::FromInt(number);
	auto* waypoint = NewObject<UWaypointComponent>(this, FName(*waypointName));
	waypoint->SetRelativeLocation(location);
	waypoint->ComponentTags.Add(FName("Waypoint"));
	if (inParent != nullptr)
	{
		waypoint->AttachTo(inParent);
	}
	waypoint->RegisterComponent();
	auto waypointCollisionName = "WaypointCollision" + FString::FromInt(number);
	auto* waypointCollision = NewObject<UBoxComponent>(this, FName(*waypointCollisionName));
	waypointCollision->ComponentTags.Add(FName("Waypoint"));
	waypointCollision->SetBoxExtent(extent);
	if (inParent != nullptr)
	{
		waypointCollision->AttachTo(waypoint);
	}
	waypointCollision->RegisterComponent();
	
	return waypoint;
}

void ANavigationVolume::DestroyChildrenComponents(USceneComponent* component)
{
	while (component->GetNumChildrenComponents() > 0)
	{
		auto childComponent = component->GetChildComponent(0);
		childComponent->UnregisterComponent();
		childComponent->DestroyComponent(true);
	}
}

void ANavigationVolume::CreateOctree(UWaypointComponent* waypoint, int32 recursion, int32 recursionIndex)
{
	if (!waypoint)
	{
		UE_LOG(LogTemp, Warning, TEXT("Waypoint is nullptr."));
		return;
	}
	if (!waypoint->HasChildBoxCollision())
	{
		UE_LOG(LogTemp, Warning, TEXT("Waypoint Has Not Child Box Collision."));
		return;
	}

	auto boxCollision = waypoint->GetChildBoxCollision();
	auto localExtent = boxCollision->GetScaledBoxExtent() / 2.0f;
	auto localOffset = boxCollision->GetScaledBoxExtent() / 2.0f;
	auto waypointName = waypoint->GetName();

	TArray<AActor*> overlappingActors;
	boxCollision->GetOverlappingActors(overlappingActors);
	
	if (overlappingActors.Num() > 0)
	{
		boxCollision->UnregisterComponent();
		boxCollision->DestroyComponent();
		waypoint->UnregisterComponent();
		waypoint->DestroyComponent();
	}
	if (recursionIndex < recursion)
	{
		recursionIndex++;
	}

	
}

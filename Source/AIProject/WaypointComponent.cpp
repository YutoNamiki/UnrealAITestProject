// Fill out your copyright notice in the Description page of Project Settings.

#include "AIProject.h"
#include "WaypointComponent.h"


// Sets default values for this component's properties
UWaypointComponent::UWaypointComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = false;
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UWaypointComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UWaypointComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

UBoxComponent* UWaypointComponent::GetChildBoxCollision()
{
	TArray<USceneComponent*, FDefaultAllocator> children;
	GetChildrenComponents(true, children);
	for (auto component : children)
	{
		auto boxCollision = Cast<UBoxComponent>(component);
		if (boxCollision == nullptr)
		{
			continue;
		}
		if (boxCollision->ComponentHasTag(FName("Waypoint")))
		{
			return boxCollision;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Component Cast Failed."));
	return nullptr;
}

bool UWaypointComponent::HasChildBoxCollision()
{
	TArray<USceneComponent*, FDefaultAllocator> children;
	GetChildrenComponents(true, children);
	for (auto component : children)
	{
		auto boxCollision = Cast<UBoxComponent>(component);
		if (boxCollision == nullptr)
		{
			continue;
		}
		if (boxCollision->ComponentHasTag(FName("Waypoint")))
		{
			return true;
		}
	}
	return false;
}

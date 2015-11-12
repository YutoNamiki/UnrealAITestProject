// Fill out your copyright notice in the Description page of Project Settings.

#include "AIProject.h"
#include "WaypointPathComponent.h"
#include "WaypointComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UWaypointPathComponent::UWaypointPathComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = false;

}

// Called when the game starts
void UWaypointPathComponent::BeginPlay()
{
	Super::BeginPlay();
	
	DrawDebugLine(GetWorld(), Waypoint1->GetComponentLocation(), Waypoint2->GetComponentLocation(), DrawPathColor, true, -1.0f, (uint8)'\000', Thickness);
}

// Called every frame
void UWaypointPathComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
}

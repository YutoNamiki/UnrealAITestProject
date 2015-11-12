// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "WaypointPathComponent.generated.h"

class UWaypointComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AIPROJECT_API UWaypointPathComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NavigationPath")
	UWaypointComponent* Waypoint1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NavigationPath")
	UWaypointComponent* Waypoint2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NavigationPath")
	FColor DrawPathColor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NavigationPath")
	float Thickness = 1.0f;

	// Sets default values for this component's properties
	UWaypointPathComponent();
	// Called when the game starts
	virtual void BeginPlay() override;
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;
};

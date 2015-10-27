// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "WaypointComponent.generated.h"


UENUM(BlueprintType)
enum class EState : uint8
{
	None	UMETA(DisplayName = "None"),
	Open	UMETA(DisplayName = "Open"),
	Closed	UMETA(DisplayName = "Closed")
};

UCLASS( ClassGroup="Waypoint", meta=(BlueprintSpawnableComponent) )
class AIPROJECT_API UWaypointComponent : public USceneComponent
{
	GENERATED_BODY()

public:	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waypoint")
	int32 ID;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Waypoint")
	EState State;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Waypoint")
	float Cost;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Waypoint")
	float Heuristic;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Waypoint")
	TArray<UWaypointComponent*> NeighborWaypoints;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Waypoint")
	UWaypointComponent* ParentWaypoint;

	// Sets default values for this component's properties
	UWaypointComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UFUNCTION(BlueprintCallable, Category = "Waypoint")
	UBoxComponent* GetChildBoxCollision();

	UFUNCTION(BlueprintCallable, Category = "Waypoint")
	bool HasChildBoxCollision();
	
};

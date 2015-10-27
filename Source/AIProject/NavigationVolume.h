// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "NavigationVolume.generated.h"

class UWaypointComponent;

UENUM(BlueprintType)
enum class EPathFindingState : uint8
{
	LineTracing				UMETA(DisplayName = "LineTrace"),
	GettingStartNode		UMETA(DisplayName = "GettingStartNode"),
	GettingEndNode			UMETA(DisplayName = "GettingEndNode"),
	PathFinding				UMETA(DisplayName = "PathFinding"),
	ConvertingPathToVector	UMETA(DisplayName = "ConvertingPathToVector")
};

UENUM(BlueprintType)
enum class EPathFindingResultState : uint8
{
	Failed		UMETA(DisplayName = "Failed"),
	Thinking	UMETA(DisplayName = "Thinking"),
	Success		UMETA(DisplayName = "Success")
};

UCLASS()
class AIPROJECT_API ANavigationVolume : public AActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "NavigationVolume")
	UBoxComponent* BoxVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waypoint")
	TArray<UWaypointComponent*> WaypointList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waypoint")
	int32 WaypointCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceDivision")
	int32 DivideX = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceDivision")
	int32 DivideY = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceDivision")
	int32 DivideZ = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceDivision")
	int32 Recursion = 0;

	// Sets default values for this actor's properties
	ANavigationVolume();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:

	int32 recursionIndex;

	void DivideVolume(UBoxComponent* volume, int32 divX, int32 divY, int32 divZ, TArray<UWaypointComponent*>& waypoints);
	UWaypointComponent* CreateWaypoint(FVector location, FVector extent, USceneComponent* inParent = nullptr);
	
};

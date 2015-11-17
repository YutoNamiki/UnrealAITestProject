// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "PathFinderInterface.h"
#include "PathFindingComponent.generated.h"

class UWaypointComponent;
class UTimeCounterComponent;

USTRUCT(BlueprintType)
struct FPathFindingInformation
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTimeCounterComponent* Timer;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UWaypointComponent*> WaypointList;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UWaypointComponent*> OpenList;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UWaypointComponent*> CloseList;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWaypointComponent* StartNode;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWaypointComponent* EndNode;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector StartLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector EndLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MaxCaluclationTime = 2.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 FindingIndex;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AIPROJECT_API UPathFindingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FPathFindingInformation PathFindInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waypoint")
	TArray<UWaypointComponent*> Waypoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PathFinding")
	float MaxCalculationTime = 2.0f;

	// Sets default values for this component's properties
	UPathFindingComponent();
	// Called when the game starts
	virtual void BeginPlay() override;
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UFUNCTION(BlueprintCallable, Category = "PathFinding")
	EPathFindingResultState FindPath(APawn* findPawn, TArray<FVector>& resultRoute, FVector start, FVector end);
	UFUNCTION(BlueprintCallable, Category = "PathFinding")
	void DrawPath(FVector start, TArray<FVector>& route, FColor color = FColor::Green, float duration = 0.0f, float thickness = 1.0f);

private:
	EPathFindingState currentState;
	bool isFindingPath;
	APawn* findingPawn;
	IPathFinderInterface* pathFinder;

	void SetStatus(bool isFinding = false, APawn* findPawn = nullptr, EPathFindingState nextState = EPathFindingState::LineTracing);

	EPathFindingResultState LineTraceForTwoLocations(TArray<FVector>& resultRoute);
	EPathFindingResultState GetStartNode();
	EPathFindingResultState GetEndNode();
	EPathFindingResultState FindPathByAStarAlgorithm();
	EPathFindingResultState ConvertRouteToVector(TArray<FVector>& route);

	UWaypointComponent* GetMinCostNode(TArray<UWaypointComponent*>& waypointList);
};

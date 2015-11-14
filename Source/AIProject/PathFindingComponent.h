// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "PathFindingComponent.generated.h"

class UWaypointComponent;
class UTimeCounterComponent;

UENUM(BlueprintType)
enum class EPathFindingState : uint8
{
	LineTracing				UMETA(DisplayName = "LineTrace"),
	GettingStartNode		UMETA(DisplayName = "GettingStartNode"),
	GettingEndNode			UMETA(DisplayName = "GettingEndNode"),
	PathFinding				UMETA(DisplayName = "PathFinding"),
	ConvertingPathToVector	UMETA(DisplayName = "ConvertingPathToVector"),
};

UENUM(BlueprintType)
enum class EPathFindingResultState : uint8
{
	Failed		UMETA(DisplayName = "Failed"),
	Thinking	UMETA(DisplayName = "Thinking"),
	Success		UMETA(DisplayName = "Success")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AIPROJECT_API UPathFindingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waypoint")
	TArray<UWaypointComponent*> Waypoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PathFinding")
	float MaxCalculationTime = 5.0f;

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
	TArray<UWaypointComponent*> openList;
	TArray<UWaypointComponent*> closeList;
	UWaypointComponent* startNode;
	UWaypointComponent* endNode;
	FVector startLocation;
	FVector endLocation;
	UTimeCounterComponent* timer;
	int findingIndex;

	void SetStatus(bool isFinding = false, APawn* findPawn = nullptr, EPathFindingState nextState = EPathFindingState::LineTracing);

	EPathFindingResultState LineTraceForTwoLocations(TArray<FVector>& resultRoute);
	EPathFindingResultState GetStartNode();
	EPathFindingResultState GetEndNode();
	EPathFindingResultState FindPathByAStarAlgorithm();
	EPathFindingResultState ConvertRouteToVector(TArray<FVector>& route);

	UWaypointComponent* GetMinCostNode(TArray<UWaypointComponent*>& waypointList);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PathFindingComponent.h"
#include "NavigationVolume.generated.h"

class UWaypointComponent;

USTRUCT(BlueprintType)
struct FWaypointPath
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NavigationPath")
	int32 WaypointID1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NavigationPath")
	int32 WaypointID2;

	void Initialize(int waypoint1, int waypoint2);
	void DrawPath();
};

UCLASS()
class AIPROJECT_API ANavigationVolume : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "NavigationVolume")
	UBoxComponent* BoxVolume;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NavigationVolume")
	UPathFindingComponent* PathFindingComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waypoint")
	TArray<UWaypointComponent*> WaypointList;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waypoint")
	TArray<FWaypointPath> WaypointPathList;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waypoint")
	int32 WaypointCount = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waypoint")
	int32 WaypointPathCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceDivision")
	int32 DivideX = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceDivision")
	int32 DivideY = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceDivision")
	int32 DivideZ = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceDivision")
	int32 Recursion = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool IsVisiblePaths = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool IsUseWaypointCollisions = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	FColor DrawPathColor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	float DrawPathDuration = 5.0f;

	// Sets default values for this actor's properties
	ANavigationVolume();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void EditorApplyTranslation(const FVector& DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;
#endif

	UFUNCTION(BlueprintCallable, Category = "PathFinding")
	EPathFindingResultState FindPath(APawn* findPawn, TArray<FVector>& resultRoute, FVector start, FVector end);

private:
	int32 recursionIndex;

	void Initialize();
	void DivideVolume(UBoxComponent* volume, int32 divX, int32 divY, int32 divZ);
	UWaypointComponent* CreateWaypoint(FVector location, FVector extent, FString name, USceneComponent* inParent = nullptr);
	void DestroyChildrenComponents(USceneComponent* component);
	void CreateOctree(UWaypointComponent* waypoint, int32 recursion, int32 recursionIndex, USceneComponent* inParent = nullptr);
	void CreatePaths(const TArray<UWaypointComponent*>& waypointList, TArray<FWaypointPath>& waypointPathList);
	void DebugSettings(TArray<UWaypointComponent*>& waypointList, TArray<FWaypointPath>& waypointPathList, bool isUseWaypointCollisions, bool isVisiblePaths);
};

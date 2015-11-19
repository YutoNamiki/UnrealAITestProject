// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PathFinderInterface.h"
#include "NavigationVolume.generated.h"

class UWaypointComponent;
class UWaypointPathComponent;
class UPathFindingComponent;

UCLASS()
class AIPROJECT_API ANavigationVolume : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "NavigationVolume")
	UBoxComponent* BoxVolume;
	UPROPERTY(BlueprintReadWrite, Category = "NavigationVolume")
	UPathFindingComponent* PathFindingComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waypoint")
	TArray<UWaypointComponent*> WaypointList;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waypoint")
	TArray<UWaypointPathComponent*> WaypointPathList;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpeceDivision")
	bool Bake = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool IsVisiblePaths = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool IsUseWaypointCollisions = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	FColor DrawPathColor = FColor::Green;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	float Thickness = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PathFinding")
	float MaxCaluclationTime = 1.0f;

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
	EPathFindingResultState FindPathByVectors(APawn* findPawn, TArray<FVector>& resultRoute, FVector start, FVector end);
	UFUNCTION(BlueprintCallable, Category = "PathFinding")
	EPathFindingResultState FindPathByActors(APawn* findPawn, TArray<FVector>& resultRoute, AActor* start, AActor* end);
	UFUNCTION(BlueprintCallable, Category = "PathFinding")
	EPathFindingResultState FindPathBySceneComponent(APawn* findPawn, TArray<FVector>& resultRoute, USceneComponent* start, USceneComponent* end);
	UFUNCTION(BlueprintCallable, Category = "PathFinding")
	void DrawPathFromVector(FVector start, TArray<FVector> route, FColor pathColor = FColor::Black, float duration = 0.0f, float pathThickness = 1.0f);
	UFUNCTION(BlueprintCallable, Category = "PathFinding")
	void DrawPathFromActor(AActor* start, TArray<FVector> route, FColor pathColor = FColor::Black, float duration = 0.0f, float pathThickness = 1.0f);
	UFUNCTION(BlueprintCallable, Category = "PathFinding")
	void DrawPathFromSceneComponent(USceneComponent* start, TArray<FVector> route, FColor pathColor = FColor::Black, float duration = 0.0f, float pathThickness = 1.0f);

private:
	int32 recursionIndex;

	void Initialize();
	void DivideVolume(UBoxComponent* volume, int32 divX, int32 divY, int32 divZ);
	UWaypointComponent* CreateWaypoint(FVector location, FVector extent, FString name, USceneComponent* inParent = nullptr);
	void DestroyChildrenComponents(USceneComponent* component);
	void CreateOctree(UWaypointComponent* waypoint, int32 recursion, int32 recursionIndex, USceneComponent* inParent = nullptr);
	void CreatePaths(const TArray<UWaypointComponent*>& waypointList, TArray<UWaypointPathComponent*>& waypointPathList, FColor color, float thickness);
	UWaypointPathComponent* CreateWaypointPath(UWaypointComponent* waypoint1, UWaypointComponent* waypoint2, FString pathName, FColor color = FColor::Green, float thickness = 1.0f);
	void DebugSettings(TArray<UWaypointComponent*>& waypointList, TArray<UWaypointPathComponent*>& waypointPathList, bool isUseWaypointCollisions, bool isVisiblePaths);
};

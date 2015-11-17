// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "PathFinderInterface.h"
#include "FindingPathByAStarAlgorithm.generated.h"

class UWaypointComponent;

UCLASS()
class AIPROJECT_API UFindingPathByAStarAlgorithm : public UObject, public IPathFinderInterface
{
	GENERATED_BODY()
	
public:
	virtual EPathFindingState FindPath(FPathFindingInformation& pathFindInfo, TArray<FVector>& resultRoute) override;
	
private:
	int32 index = 0;

	UWaypointComponent* GetMinCostNode(TArray<UWaypointComponent*>& waypointList);
};

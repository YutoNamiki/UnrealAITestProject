// Fill out your copyright notice in the Description page of Project Settings.

#include "AIProject.h"
#include "LineTracingForStartAndEnd.h"
#include "PathFindingComponent.h"

EPathFindingState ULineTracingForStartAndEnd::FindPath(FPathFindingInformation& pathFindInfo, TArray<FVector>& resultRoute)
{
	if (GetWorld()->LineTraceTestByObjectType(pathFindInfo.StartLocation, pathFindInfo.EndLocation, FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic)))
		return EPathFindingState::GettingStartNode;
	resultRoute.Empty();
	resultRoute.Add(pathFindInfo.EndLocation);
	return EPathFindingState::Finished;
}

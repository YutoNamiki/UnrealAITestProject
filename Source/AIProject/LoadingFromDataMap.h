// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "PathFinderInterface.h"
#include "LoadingFromDataMap.generated.h"

UCLASS()
class AIPROJECT_API ULoadingFromDataMap : public UObject, public IPathFinderInterface
{
	GENERATED_BODY()
	
public:
	virtual EPathFindingState FindPath(UWorld* world, FPathFindingInformation& pathFindInfo, TArray<FVector>& resultRoute) override;
	
private:
	int32 index = 0;
};

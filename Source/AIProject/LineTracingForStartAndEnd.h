// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "PathFinderInterface.h"
#include "LineTracingForStartAndEnd.generated.h"

UCLASS()
class AIPROJECT_API ULineTracingForStartAndEnd : public UObject, public IPathFinderInterface
{
	GENERATED_BODY()

public:
	virtual EPathFindingState FindPath(FPathFindingInformation& pathFindInfo, TArray<FVector>& resultRoute) override;
};

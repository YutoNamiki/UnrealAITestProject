// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "MyTestObject.generated.h"

class UWorld;

UCLASS()
class AIPROJECT_API UMyTestObject : public UObject
{
	GENERATED_BODY()
	
public:
	void Function(UWorld* world);
	
};

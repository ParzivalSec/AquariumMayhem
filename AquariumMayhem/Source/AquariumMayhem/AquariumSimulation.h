// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AquariumMayhemGameModeBase.h"
#include "AquariumSimulation.generated.h"

/**
 * 
 */
UCLASS()
class AQUARIUMMAYHEM_API AAquariumSimulation : public AAquariumMayhemGameModeBase
{
	GENERATED_BODY()
	
public:
	AAquariumSimulation();
	void BeginPlay() override;
	
};

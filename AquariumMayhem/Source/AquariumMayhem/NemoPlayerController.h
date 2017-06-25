// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ContainerCtrl.h"
#include "NemoPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class AQUARIUMMAYHEM_API ANemoPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	AContainerCtrl* SimulationContainers[4];
	int ActiveContainerIndex;

	void ResetContainer();
	void ToggleEmitter();
	void NextContainer();
	void PreviousContainer();
	void SwitchMaterial();

	void SwitchCamera();


public:
	// Called to bind functionality to input
	virtual void SetupInputComponent() override;
};

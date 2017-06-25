// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ContainerCtrl.generated.h"

UENUM()
enum class EGlassType : uint8
{
	MM_3_UNTREATED,
	MM_3_TREATED,
	MM_5_UNTREATED,
	MM_5_TREATED
};

UCLASS()
class AQUARIUMMAYHEM_API AContainerCtrl : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AContainerCtrl();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<float> SectorHeights;

	UPROPERTY()
	class UParticleSystemComponent* FluidEmitter;

	float BreakingPressure;

	float CalculateWaterHeight();
	float CalculateWaterPressureAtPoint(float height);
	float CalculateTensibleStrengthFive(bool treated);
	float CalculateTensibleStrengthThree(bool treated);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ToggleEmitter();
	
	UPROPERTY(EditAnywhere, Category = "Controller Props")
	class UFlexContainer* FluidContainer;

	UPROPERTY(EditAnywhere, Category = "Controller Props")
	float ContainerVolume;

	UPROPERTY(EditAnywhere, Category = "Controller Props")
	float FluidDensity;

	UPROPERTY(EditAnywhere, Category = "Controller Props")
	float WallDensity;

	UPROPERTY(EditAnywhere, Category = "Controller Props")
	EGlassType GlassType;

	UPROPERTY(EditAnywhere, Category = "Controller Props")
	float WallHeight;

	UPROPERTY(EditAnywhere, Category = "Controller Props")
	float WallWidth;

	UPROPERTY(EditAnywhere, Category = "Controller Props")
	UINT32 WallSectors;

	UPROPERTY(EditAnywhere, Category = "Controller Props")
	UINT32 MaxContainerParticles;
};

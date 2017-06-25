// Fill out your copyright notice in the Description page of Project Settings.

#include "ContainerCtrl.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicsEngine/RadialForceActor.h"
#include "Components/DestructibleComponent.h"

// Sets default values
AContainerCtrl::AContainerCtrl()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AContainerCtrl::BeginPlay()
{
	Super::BeginPlay();
	FVector actorPos = GetActorLocation();

	// Get ref to particle emitter
	FluidEmitter = Cast<UParticleSystemComponent>(GetComponentByClass(UParticleSystemComponent::StaticClass()));
	
	float sectorHeight = WallHeight / static_cast<float>(WallSectors);

	// Calculate pressure measure points
	for (size_t sec = 0; sec < WallSectors; sec++)
	{
		float measureHeight = actorPos.Z + sectorHeight * sec;
		SectorHeights.Add(measureHeight);
	}

	switch(GlassType)
	{
		case EGlassType::MM_3_UNTREATED:
			BreakingPressure = CalculateTensibleStrengthThree(false);
			break;

		case EGlassType::MM_3_TREATED:
			BreakingPressure = CalculateTensibleStrengthThree(true);
			break;

		case EGlassType::MM_5_UNTREATED:
			BreakingPressure = CalculateTensibleStrengthFive(false);
			break;

		case EGlassType::MM_5_TREATED:
			BreakingPressure = CalculateTensibleStrengthFive(true);
			break;

		default:
			break;
	}

	UE_LOG(LogTemp, Warning, TEXT("Breaking Pressure: %f"), BreakingPressure)
}

float AContainerCtrl::CalculateWaterHeight()
{
	float ratio = WallHeight / MaxContainerParticles;
	return GetActorLocation().Z + FluidEmitter->GetNumActiveParticles() * ratio;
}

/**
 * Calculate water pressurce in Pascal (Pa) at a given height
 */
float AContainerCtrl::CalculateWaterPressureAtPoint(float height)
{
	return (CalculateWaterHeight() - height) * FluidDensity * 9.81f;
}

float AContainerCtrl::CalculateTensibleStrengthFive(bool treated)
{
	float a, b;
	
	if (treated)
	{
		a = 7.51717f;
		b = -0.5038082;
	}
	else
	{
		a = 5.801743f;
		b = -0.6798752f;
	}

	float x = (WallWidth * WallHeight) / 10000.0f;

	// Result conersion from kN/m^2 to Pascal (N/m^2)
	return (a * FMath::Pow(x, b)) * 1000.0f;
}

float AContainerCtrl::CalculateTensibleStrengthThree(bool treated)
{
	float a, b;

	if (treated)
	{
		a = 4.177024f;
		b = -0.6715691f;
	}
	else
	{
		a = 3.75943f;
		b = -0.7399408;
	}

	float x = (WallWidth * WallHeight) / 10000.0f;

	// Result conersion from kN/m^2 to Pascal (N/m^2)
	return (a * FMath::Pow(x, b)) * 1000.0f;
}

// Called every frame
void AContainerCtrl::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float currentHeight = CalculateWaterHeight();
	FVector swimmerLocation = GetActorLocation();
	swimmerLocation.Z = currentHeight;
	DrawDebugSphere(GetWorld(), swimmerLocation, 12, 32, FColor(0, 255,  0));

	// Draw debug spheres
	for (size_t i = 0; i < SectorHeights.Num(); i++)
	{
		if (CalculateWaterPressureAtPoint(SectorHeights[i]) >= BreakingPressure)
		{
			swimmerLocation.Z = SectorHeights[i];
			FRotator rot;
			UE_LOG(LogTemp, Error, TEXT("Glass will break at measure point: %i"), i)
			//ARadialForceActor* radialForce = Cast<ARadialForceActor>(GetWorld()->SpawnActor(ARadialForceActor::StaticClass(), &swimmerLocation, &rot));
			//radialForce->FireImpulse();
			UDestructibleComponent* mesh = Cast<UDestructibleComponent>(GetComponentByClass(UDestructibleComponent::StaticClass()));
			if (mesh)
			{
				FVector hitOrigin = mesh->GetComponentLocation();
				hitOrigin.Z = SectorHeights[i];
				mesh->ApplyRadiusDamage(10.0f, hitOrigin + mesh->GetRightVector() * WallWidth * 0.25f, 10.0f, 100.0f, false);
				mesh->ApplyRadiusDamage(10.0f, hitOrigin, 10.0f, 100.0f, false);
				mesh->ApplyRadiusDamage(10.0f, hitOrigin - mesh->GetRightVector() * WallWidth * 0.25f, 10.0f, 100.0f, false);
				FluidEmitter->DeactivateSystem();
			}
		}
	}

}

void AContainerCtrl::ToggleEmitter()
{
	static bool bActive = true;

	if (bActive)
	{
		FluidEmitter->DeactivateSystem();
	}
	else
	{
		FluidEmitter->ActivateSystem();
	}

	bActive = !bActive;
}


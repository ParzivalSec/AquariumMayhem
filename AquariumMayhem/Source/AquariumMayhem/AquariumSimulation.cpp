// Fill out your copyright notice in the Description page of Project Settings.

#include "AquariumSimulation.h"


AAquariumSimulation::AAquariumSimulation()
{
	// Can be used to not spawn a Pawn when the GameModeBase wants to
	// Only the PlayerController is spawned now 
	// (also all UGameplayStatics::CreatePlayer calls spawn only a PlayerController now)
	DefaultPawnClass = nullptr;
}

void AAquariumSimulation::BeginPlay()
{
	Super::BeginPlay();
}
